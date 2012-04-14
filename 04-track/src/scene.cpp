#include <QtGui>
#include <QtDebug>

#include "scene.h"
#include "wheel.h"

const QRect Scene::DEFAULT_SCENE_RECT = QRect(0, 0, 900, 600);
const QColor Scene::BACKGROUND_COLOR = QColor("#DAEDDC");

const QPointF Scene::INITIAL_POS = QPointF(300, 400);
const int Scene::TRACK_STEP = 5;
const double Scene::TRACK_WIDTH = 300;

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
    setBackgroundBrush(BACKGROUND_COLOR);
    setSceneRect(DEFAULT_SCENE_RECT);

    setup();
}

void Scene::setup() {
    addItem(new QGraphicsLineItem(0, INITIAL_POS.y(), sceneRect().width(), INITIAL_POS.y()));
    addWheel(INITIAL_POS);
    addWheel(INITIAL_POS + QPointF(TRACK_WIDTH, 0));
}

void Scene::addWheel(QPointF pos) {
    Wheel *wheel = new Wheel();
    wheel->setPos(pos - QPointF(0, wheel->radius()));
    m_trackParts << wheel;
    addItem(wheel);
}

void Scene::moveTrack(double step) {
    foreach (MovableItemGroup *part, m_trackParts) {
        part->move(step);
    }
}

void Scene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Left:
            moveTrack(-TRACK_STEP);
            break;
        case Qt::Key_Right:
            moveTrack(TRACK_STEP);
            break;
    }

    QGraphicsScene::keyPressEvent(event);
}
