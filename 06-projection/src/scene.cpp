#include <QtGui>
#include <QtDebug>

#include "scene.h"
#include "pixmapitem.h"
#include "projection.h"

const QRect Scene::DEFAULT_SCENE_RECT = QRect(0, 0, 900, 600);
const QColor Scene::BACKGROUND_COLOR = QColor("#DAEDDC");

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
    setBackgroundBrush(BACKGROUND_COLOR);
    setSceneRect(DEFAULT_SCENE_RECT);

    m_pixmap = new PixmapItem(sceneRect().size().toSize(), this);

    Projection p;
    QMatrix4x4 m = p.transformMatrix();

    qDebug() << p.transformPoint(QVector3D(-3, 1, 1), m);
    qDebug() << p.transformPoint(QVector3D(3, 1, 1), m);
    qDebug() << p.transformPoint(QVector3D(-3, 2, 1), m);

    qDebug() << p.transformPoint(QVector3D(-3, 1, 2), m);
    qDebug() << p.transformPoint(QVector3D(-3, 2, 1), m);
    qDebug() << p.transformPoint(QVector3D(-4, 1, 1), m);
}
