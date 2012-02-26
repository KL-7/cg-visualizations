#include <QtGui>
#include <QDebug>

#include "scene.h"
#include "rect.h"
#include "quadratictree.h"
#include "pointspixmapitem.h"

const QRect Scene::DEFAULT_SCENE_RECT = QRect(0, 0, 900, 600);
const QRect Scene::RECT_GEOMETRY = QRect(0, 0, 220, 180);
const double Scene::POINT_RADIUS = 1;
const int Scene::POINTS_COUNT = 500000;
const QColor Scene::INNER_POINT_COLOR = Qt::yellow;
const QColor Scene::OUTER_POINT_COLOR = Qt::black;

Scene::Scene(QObject *parent) : QGraphicsScene(parent), m_fast(false) {
    setBackgroundBrush(QColor("#DAEDDC"));
    setSceneRect(DEFAULT_SCENE_RECT);

    m_pixmap = new PointsPixmapItem(sceneRect().size().toSize(), this);

    addRect();
    addPoints();
}

void Scene::addPoints(int count) {
    for (int i = 0; i < count; ++i) {
        QPointF point(rand(width()), rand(height()));
        m_points.append(point);
    }

    m_tree = new QuadraticTree(sceneRect(), m_points);

//    drawQuadraticTree();
    recolorPoints();
}

void Scene::drawQuadraticTree() {
    QQueue<Node*> nodes;
    nodes.enqueue(m_tree->root());

    while (!nodes.empty()) {
        Node *node = nodes.dequeue();
        QGraphicsRectItem *rect = new QGraphicsRectItem(node->rect);
        rect->setPen(QPen(Qt::DotLine));
        addItem(rect);
        foreach(Node *child, node->children) {
            nodes.enqueue(child);
        }
    }
}

double Scene::rand(double max) {
    return max * qrand() / RAND_MAX;
}

QSize Scene::rectSize() const {
    return m_rect->rect().size().toSize();
}

int Scene::pointsCount() const {
    return m_points.size();
}

void Scene::addRect()
{
    m_rect = new Rect(RECT_GEOMETRY, 0, this);
    m_rect->setZValue(-1);
}

void Scene::removePoints() {
    m_points.clear();
    m_pixmap->clear();
}

void Scene::regenerate(int pointsCount, QSize rectSize) {
    setRectSize(rectSize);
    regeneratePoints(pointsCount);
}

void Scene::regeneratePoints(int count) {
    removePoints();
    addPoints(count);
}

void Scene::setRectSize(QSize rectSize) {
    m_rect->setRect(QRect(QPoint(0, 0), rectSize));
}

void Scene::switchMode(bool fast) {
    m_fast = fast;
    if (m_fast) {
        m_tree->pointsInsideRect(m_rect->sceneBoundingRect(), m_pointsInsideRect);
    }
}

void Scene::recolorPoints() {
    QList<QPointF> innerPoints, outerPoints;
    QRectF rect = m_rect->mapToScene(m_rect->rect()).boundingRect();

    if (m_fast) {
        outerPoints = m_pointsInsideRect;
        m_tree->pointsInsideRect(m_rect->sceneBoundingRect(), m_pointsInsideRect);
        innerPoints = m_pointsInsideRect;
    } else {
        foreach (QPointF point, m_points) {
            if (rect.contains(point)) {
                innerPoints.append(point);
            } else {
                outerPoints.append(point);
            }
        }
    }

    m_pixmap->drawPoints(outerPoints, OUTER_POINT_COLOR);
    m_pixmap->drawPoints(innerPoints, INNER_POINT_COLOR);
}
