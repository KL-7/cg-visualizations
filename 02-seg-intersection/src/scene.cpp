#include <QtGui>
#include <QDebug>

#include "scene.h"
#include "pixmapitem.h"
#include "bentleyottmann.h"

const QRect Scene::DEFAULT_SCENE_RECT = QRect(0, 0, 1400, 850);

const int Scene::SEGMENTS_COUNT = 20000;

const int Scene::POINT_RADIUS = 2;

const double Scene::SEGMENT_LENGTH = 30;

const QColor Scene::SEGMENT_COLOR = Qt::black;
const QColor Scene::INTERSECTION_COLOR = Qt::blue;

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
    setBackgroundBrush(QColor("#DAEDDC"));
    setSceneRect(DEFAULT_SCENE_RECT);
    m_pixmap = new PixmapItem(sceneRect().size().toSize(), this);
    regenerateSegments();
}

void Scene::generateSegments(int count) {
    m_segments.clear();

    qsrand(QTime::currentTime().msec());

    for (int i = 0; i < count; ++i) {
        QPointF point(rand(width()), rand(height()));
        QLineF segment(point, point + QPointF(SEGMENT_LENGTH, 0));
        segment.setAngle(rand(360));
        m_segments << segment;
    }
}

void Scene::regenerateSegments() {
    generateSegments();
    redrawSegments();
}

void Scene::redrawSegments() {
    m_pixmap->clear();
    m_pixmap->drawSegments(m_segments, SEGMENT_COLOR);
}

void Scene::addsegments() {
    m_segments
            << QLineF(75, 0, 175, 100)
            << QLineF(25, 25, 150, 50)
            << QLineF(50, 125, 125, 75)
               ;

    m_pixmap->drawSegments(m_segments, SEGMENT_COLOR);
}

double Scene::rand(double max) {
    return max * qrand() / RAND_MAX;
}

void Scene::colorIntersections() {
    redrawSegments();

    QVector<QPointF> points;
    QPointF point;

    QTime timer;
    timer.start();

    for (int i = 0; i < m_segments.size() - 1; ++i) {
        for (int j = i + 1; j < m_segments.size(); ++j) {
            if (QLineF::BoundedIntersection == m_segments[i].intersect(m_segments[j], &point)) {
                points << point;
            }
        }
    }

    qDebug() << "slow: " << timer.elapsed() << "ms";

    m_pixmap->drawPoints(points, INTERSECTION_COLOR, POINT_RADIUS);
}

void Scene::fastColorIntersections() {
    redrawSegments();

    BentleyOttmann alg(m_segments);

    QTime timer;
    timer.start();

    QVector<QPointF> points = alg.intersectionPoints();

    qDebug() << "fast: " << timer.elapsed() << "ms";

    m_pixmap->drawPoints(points, INTERSECTION_COLOR, POINT_RADIUS);
}
