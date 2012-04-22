#include <QtGui>

#include "scene.h"
#include "pixmapitem.h"
#include "graham.h"

const QRect Scene::DEFAULT_SCENE_RECT = QRect(0, 0, 900, 600);
const QColor Scene::BACKGROUND_COLOR = QColor("#DAEDDC");
const int Scene::POINTS_NUMBER = 500;
const int Scene::PADDING = 40;

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
    setBackgroundBrush(BACKGROUND_COLOR);
    setSceneRect(DEFAULT_SCENE_RECT);

    m_pixmap = new PixmapItem(sceneRect().size().toSize(), this);
}

void Scene::generatePoints(int pointsNumber) {
    m_pixmap->clear();
    m_points.clear();

    for (int i = 0; i < pointsNumber; ++i) {
        m_points.append(QPointF(rand(width()), rand(height())));
    }

    m_pixmap->drawPoints(m_points);
}

double Scene::rand(double range) {
    return PADDING + (range - 2 * PADDING) * qrand() / RAND_MAX;
}

void Scene::buildConvexHull() {
    Graham alg(m_points);
    QList<QPointF> hull = alg.convexHull();
    int hullSize = hull.size();

    QList<QLineF> lines;

    for (int i = 0; i < hullSize; ++i) {
        lines.append(QLineF(hull[i], hull[(i + 1) % hullSize]));
    }

    m_pixmap->drawLines(lines);
}
