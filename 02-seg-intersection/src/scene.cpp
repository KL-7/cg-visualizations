#include <QtGui>
#include <QDebug>

#include "scene.h"
#include "pixmapitem.h"

const QRect Scene::DEFAULT_SCENE_RECT = QRect(0, 0, 900, 600);
const int Scene::SEGMENTS_COUNT = 10000;
const int Scene::POINT_RADIUS = 2;
const double Scene::SEGMENT_LENGTH = 20;
const QColor Scene::SEGMENT_COLOR = Qt::black;
const QColor Scene::INTERSECTION_COLOR = Qt::blue;

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
    setBackgroundBrush(QColor("#DAEDDC"));
    setSceneRect(DEFAULT_SCENE_RECT);
    m_pixmap = new PixmapItem(sceneRect().size().toSize(), this);
    generateSegments();
}

void Scene::generateSegments(int count) {
    for (int i = 0; i < count; ++i) {
        QPointF point(rand(width()), rand(height()));
        QLineF segment(point, point + QPointF(SEGMENT_LENGTH, 0));
        segment.setAngle(rand(360));
        m_segments << segment;
    }

    m_pixmap->drawSegments(m_segments, SEGMENT_COLOR);
}

double Scene::rand(double max) {
    return max * qrand() / RAND_MAX;
}

void Scene::colorIntersections() {
    QVector<QPointF> points;
    QPointF point;
    for (int i = 0; i < m_segments.size() - 1; ++i) {
        for (int j = i + 1; j < m_segments.size(); ++j) {
            if (QLineF::BoundedIntersection == m_segments[i].intersect(m_segments[j], &point)) {
                points << point;
            }
        }
    }

    m_pixmap->drawPoints(points, INTERSECTION_COLOR, POINT_RADIUS);
}
