#include <QtGui>
#include <QtDebug>

#include "scene.h"
#include "projection.h"

const QRect Scene::DEFAULT_SCENE_RECT = QRect(0, 0, 900, 600);
const QColor Scene::BACKGROUND_COLOR = QColor("#DAEDDC");

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
    setBackgroundBrush(BACKGROUND_COLOR);
    setSceneRect(DEFAULT_SCENE_RECT);

    Projection p;

    p.render(this);

//    addItem(new QGraphicsLineItem(QLineF(300, 300, 400, 400)));

//    QMatrix4x4 m = p.transformMatrix();

//    qDebug() << p.transformPoint3D(QVector3D(-3, 1, 1), m);
//    qDebug() << p.transformPoint3D(QVector3D(2, 1, 1), m);

//    qDebug() << p.transformPoint3D(QVector3D(-2, 0, 0), m);
//    qDebug() << p.transformPoint3D(QVector3D(-1, -1, -1), m);
//    qDebug() << p.transformPoint3D(QVector3D(0, -2, -2), m);
//    qDebug() << p.transformPoint3D(QVector3D(2, -4, -4), m);
}
