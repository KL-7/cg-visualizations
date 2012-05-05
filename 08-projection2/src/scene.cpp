#include <QtGui>
#include <QtDebug>

#include "scene.h"
#include "projection.h"

const QRect Scene::DEFAULT_SCENE_RECT = QRect(0, 0, 900, 600);
const QColor Scene::BACKGROUND_COLOR = QColor("#DAEDDC");

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
    setBackgroundBrush(BACKGROUND_COLOR);
    setSceneRect(DEFAULT_SCENE_RECT);

    projection = new Projection(this);

    projection->render();
}

void Scene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {

    case Qt::Key_A:
        projection->move(-1, 0);
        break;
    case Qt::Key_D:
        projection->move(1, 0);
        break;
    case Qt::Key_S:
        projection->move(0, -1);
        break;
    case Qt::Key_W:
        projection->move(0, 1);
        break;

    case Qt::Key_Equal:
        projection->zoom(1);
        break;
    case Qt::Key_Minus:
        projection->zoom(-1);
        break;

    case Qt::Key_Left:
        projection->rotate(-1, 0);
        break;
    case Qt::Key_Right:
        projection->rotate(1, 0);
        break;
    case Qt::Key_Down:
        projection->rotate(0, -1);
        break;
    case Qt::Key_Up:
        projection->rotate(0, 1);
        break;

    }

    projection->render();
}
