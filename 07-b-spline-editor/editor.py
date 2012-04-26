#!/usr/bin/env python

import math
import bspline

from PySide.QtCore import *
from PySide.QtGui import *


class ControlPoint(QGraphicsEllipseItem):

    PointRadius = 5
    BackgroundColor = QColor('#BCF1F7')

    def __init__(self, number, x, y, spline, *args, **kwargs):
        super(ControlPoint, self).__init__(*args, **kwargs)

        self.setFlags(QGraphicsItem.ItemIsMovable |
                      QGraphicsItem.ItemSendsGeometryChanges)

        self.segments = []
        self.spline = spline
        self.number = number
        self.label = QGraphicsTextItem(str(number), self)

        self.setBrush(self.BackgroundColor)

        self.setPos(x, y)
        self.setRect(-self.PointRadius, -self.PointRadius,
                     2 * self.PointRadius, 2 * self.PointRadius)

    def addSegment(self, segment):
        self.segments.append(segment)

    def itemChange(self, change, value):
        if change == QGraphicsItem.ItemPositionHasChanged:
            for segment in self.segments:
                segment.trackPoints()
            self.spline.updateSpline()

        return super(ControlPoint, self).itemChange(change, value)


class Segment(QGraphicsLineItem):

    def __init__(self, startPoint, endPoint, *args, **kwargs):
        super(Segment, self).__init__(*args, **kwargs)

        self.setZValue(-1)

        self.startPoint = startPoint
        self.startPoint.addSegment(self)
        self.endPoint = endPoint
        self.endPoint.addSegment(self)

        self.trackPoints()

    def trackPoints(self):
        self.setLine(QLineF(self.startPoint.pos(), self.endPoint.pos()))


class BSpline(QGraphicsPathItem):

    def __init__(self, degree, controlPointsNumber, width, height, renderingStepsNumber=200, *args, **kwargs):
        super(BSpline, self).__init__(*args, **kwargs)

        self.width = width
        self.height = height
        self.controlPoints = []
        self.segments = []
        self.renderingStepsNumber = renderingStepsNumber
        self.controlPointsNumber = controlPointsNumber
        self.degree = min(degree, self.maxDegree())
        self.generateConrolPoints()

        self.updateSpline()

    def setRenderingStepsNumber(self, renderingStepsNumber):
        self.renderingStepsNumber = renderingStepsNumber
        self.updateSpline()

    def setDegree(self, degree):
        if degree == self.degree:
            return

        self.degree = degree
        self.updateSpline()

    def setControlPointsNumber(self, controlPointsNumber):
        if controlPointsNumber == self.controlPointsNumber or \
           controlPointsNumber < 2:
            return

        self.controlPointsNumber = controlPointsNumber
        self.generateConrolPoints()
        if self.degree > self.maxDegree():
            self.degree = self.maxDegree()

        self.updateSpline()

    def maxDegree(self):
        return self.controlPointsNumber - 1

    def generateConrolPoints(self):
        self.circleControlPoints()

    def circleControlPoints(self):
        if self.scene():
            for controlPoint in self.controlPoints:
                self.scene().removeItem(controlPoint)
            for segment in self.segments:
                self.scene().removeItem(segment)

        self.controlPoints = []
        self.segments = []

        r = min(self.width, self.height) / 3.0

        step = 2 * math.pi / self.controlPointsNumber

        for i in xrange(self.controlPointsNumber):
            alpha = i * step
            x = self.width / 2 + r * math.cos(alpha)
            y = self.height / 2 + r * math.sin(alpha)
            point = ControlPoint(i + 1, x, y, self, parent=self)

            self.controlPoints.append(point)

            if len(self.controlPoints) > 1:
                segment = Segment(self.controlPoints[-2],
                                  self.controlPoints[-1],
                                  parent=self)
                self.segments.append(segment)


    def updateSpline(self):
        if self.controlPointsNumber != len(self.controlPoints):
            return

        cp2xy = lambda p: (p.scenePos().x(), p.scenePos().y())
        curve = bspline.bspline(map(cp2xy, self.controlPoints), self.degree)

        points = map(curve, (i / float(self.renderingStepsNumber)
                             for i in xrange(self.renderingStepsNumber + 1)))

        path = QPainterPath()
        path.addPolygon(QPolygonF([QPointF(x, y) for x, y in points]))

        self.setPath(path)

    def __str__(self):
        info = []
        info.append('B-Spline')
        info.append('Degree: %d' % self.degree)
        info.append('Control points number: %d' % self.controlPointsNumber)
        cp2xy = lambda p: '(%2.3f, %2.3f)' % (p.scenePos().x(), self.height - p.scenePos().y())
        info.append('Control points: [%s]' % ' '.join(map(cp2xy, self.controlPoints)))

        return '\n'.join(info)


class SplineScene(QGraphicsScene):

    BackgroundColor = QColor('#DFFCDE')
    GridColor = QColor('#99C997')
    AxisColor = QColor('#000000')
    AxisArrowDX = 5
    AxisArrowDY = 4
    SceneRect = QRect(0, 0, 980, 700)
    GridXStep = 70
    GridYStep = 70

    def __init__(self, splineDegree, controlPointsNumber, SplineType, *args, **kwargs):
        super(SplineScene, self).__init__(*args, **kwargs)

        self.setSceneRect(self.SceneRect)
        self.setBackgroundBrush(self.BackgroundColor)

        self.addCoordinateSystem()
        self.spline = SplineType(splineDegree, controlPointsNumber,
                                  self.width(), self.height())
        self.addItem(self.spline)

    def addCoordinateSystem(self):
        self.addGrid()
        self.addAxis()

    def addGrid(self):
        pen = QPen(self.GridColor)

        x = 0
        while x <= self.width():
            self.addLine(x, 0, x, self.height(), pen)
            if x:
                label = self.addText(str(x))
                label.setPos(x - label.boundingRect().width() / 2,
                             self.height())
            x += self.GridXStep

        y = 0
        while y <= self.height():
            self.addLine(0, y, self.width(), y, pen)
            if self.height() - y:
                label = self.addText(str((self.height() - y)))
                label.setPos(-label.boundingRect().width() - 4,
                             y - label.boundingRect().height() / 2)
            y += self.GridYStep

        label = self.addText('0')
        label.setPos(-label.boundingRect().width() - 4,
                     self.height())

    def addAxis(self):
        pen = QPen(self.AxisColor, 2)

        leftEnd = QPointF(-self.GridXStep, self.height())
        rightEnd = QPointF(self.width() + self.GridXStep, self.height())
        self.addLine(QLineF(leftEnd, rightEnd), pen)
        self.addLine(QLineF(rightEnd, rightEnd +
                            QPointF(-self.AxisArrowDX, -self.AxisArrowDY)), pen)
        self.addLine(QLineF(rightEnd, rightEnd +
                            QPointF(-self.AxisArrowDX, self.AxisArrowDY)), pen)

        topEnd = QPointF(0, -self.GridYStep)
        bottomEnd = QPointF(0, self.height() + self.GridYStep)
        self.addLine(QLineF(bottomEnd, topEnd), pen)
        self.addLine(QLineF(topEnd, topEnd +
                            QPointF(-self.AxisArrowDY, self.AxisArrowDX)), pen)
        self.addLine(QLineF(topEnd, topEnd +
                            QPointF(self.AxisArrowDY, self.AxisArrowDX)), pen)

    def saveImage(self, fileName):
        width = self.width() + 3 * self.GridXStep
        height = self.height() + 3 * self.GridYStep
        img = QImage(width, height, QImage.Format_ARGB32_Premultiplied)
        painter = QPainter()
        painter.begin(img)
        painter.setRenderHints(QPainter.Antialiasing | QPainter.TextAntialiasing)
        self.render(painter, source=QRectF(-1.5 * self.GridXStep, -1.5 * self.GridYStep, width, height))
        painter.end()
        img.save(fileName)

    def saveData(self, fileName):
        open(fileName, 'w').write(str(self.spline))


class WheelZoomGraphicsView(QGraphicsView):

    ZoomFactor = 1.2

    def __init__(self, *args, **kwargs):
        super(WheelZoomGraphicsView, self).__init__(*args, **kwargs)

        self.setRenderHints(QPainter.Antialiasing |
                            QPainter.TextAntialiasing)
        self.zoom = 1

    def wheelEvent(self, event):
        if event.modifiers() & Qt.ControlModifier:
            numDegrees = event.delta() / 8.0
            numSteps = numDegrees / 15.0
            factor = 1.125 ** numSteps
            self.scale(factor, factor)
            self.zoom * factor
        else:
            super(WheelZoomGraphicsView, self).wheelEvent(event)

    def zoomIn():
        self.scale(self.ZoomFactor, self.ZoomFactor)
        self.zoom *= self.ZoomFactor

    def zoomOut():
        self.scale(1 / self.ZoomFactor, 1 / self.ZoomFactor)
        self.zoom /= self.ZoomFactor

    def zoomNormal():
        self.scale(1 / self.zoom, 1 / self.zoom)
        self.zoom = 1


class SplineEditorWidget(QWidget):

    DefaultSplineDegree = 4
    DefaultControlPointsNumber = 10
    DefaultRenderingStepsNumber = 200

    def __init__(self, SplineType, *args, **kwargs):
        super(SplineEditorWidget, self).__init__(*args, **kwargs)

        self.scene = SplineScene(self.DefaultSplineDegree, self.DefaultControlPointsNumber,
                                 SplineType)
        self.view = WheelZoomGraphicsView(self.scene)

        self.splineDegreeSpinBox = QSpinBox()
        self.splineDegreeSpinBox.setValue(self.scene.spline.degree)
        self.splineDegreeSpinBox.valueChanged.connect(
            self.changeSplineDegree)

        self.controlPointsNumberSpinBox = QSpinBox()
        self.controlPointsNumberSpinBox.setRange(1, 100)
        self.controlPointsNumberSpinBox.valueChanged.connect(
            self.changeControlPointsNumber)
        self.controlPointsNumberSpinBox.setValue(self.scene.spline.controlPointsNumber)

        self.renderingStepsNumberSpinBox = QSpinBox()
        self.renderingStepsNumberSpinBox.setRange(20, 1000)
        self.renderingStepsNumberSpinBox.setSingleStep(20)
        self.renderingStepsNumberSpinBox.valueChanged.connect(
            self.changeRenderingStepsNumber)
        self.renderingStepsNumberSpinBox.setValue(self.scene.spline.renderingStepsNumber)

        self.saveImageButton = QPushButton('Save image...')
        self.saveImageButton.clicked.connect(self.saveImage)

        self.saveDataButton = QPushButton('Save data...')
        self.saveDataButton.clicked.connect(self.saveData)

        controlsLayout = QHBoxLayout()
        controlsLayout.addWidget(QLabel('Spline degree:'))
        controlsLayout.addWidget(self.splineDegreeSpinBox)
        controlsLayout.addWidget(QLabel('Control points number:'))
        controlsLayout.addWidget(self.controlPointsNumberSpinBox)
        controlsLayout.addWidget(QLabel('Rendering steps number:'))
        controlsLayout.addWidget(self.renderingStepsNumberSpinBox)
        controlsLayout.addStretch(1)
        controlsLayout.addWidget(self.saveImageButton)
        controlsLayout.addWidget(self.saveDataButton)

        mainLayout = QVBoxLayout()
        mainLayout.addWidget(self.view)
        mainLayout.addLayout(controlsLayout)

        self.setLayout(mainLayout)

    def changeControlPointsNumber(self, controlPointsNumber):
        controlPointsNumber = int(controlPointsNumber)
        self.scene.spline.setControlPointsNumber(controlPointsNumber)
        self.splineDegreeSpinBox.setRange(1, self.scene.spline.maxDegree())
        if self.scene.spline.degree != self.splineDegreeSpinBox:
            self.splineDegreeSpinBox.setValue(self.scene.spline.degree)

    def changeSplineDegree(self, splineDegree):
        self.scene.spline.setDegree(int(splineDegree))

    def changeRenderingStepsNumber(self, renderingStepsNumber):
        self.scene.spline.setRenderingStepsNumber(int(renderingStepsNumber))

    def saveImage(self):
        fileName = QFileDialog.getSaveFileName(self, 'Save Image', '.',
                                               'PNG Files (*.png);;All Files(*.*)')[0]
        if fileName:
            if not fileName.lower().endswith('.png'):
                fileName += '.png'
            self.scene.saveImage(fileName)

    def saveData(self):
        fileName = QFileDialog.getSaveFileName(self, 'Save Image', '.', 'All Files(*.*)')[0]
        if fileName:
            self.scene.saveData(fileName)


if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)

    w = SplineEditorWidget(BSpline)
    w.show()

    sys.exit(app.exec_())
