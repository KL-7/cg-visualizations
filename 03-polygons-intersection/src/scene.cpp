#include <QtGui>
#include <QDebug>

#include "scene.h"

const QRect Scene::DEFAULT_SCENE_RECT = QRect(0, 0, 900, 600);
const QColor Scene::SEGMENT_COLOR = Qt::black;
const QColor Scene::INTERSECTION_COLOR = Qt::blue;
const QColor Scene::BACKGROUND_COLOR = QColor("#DAEDDC");

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
    setBackgroundBrush(BACKGROUND_COLOR);
    setSceneRect(DEFAULT_SCENE_RECT);
}

void Scene::addPolygons() {

}

void Scene::findIntersection() {

}
