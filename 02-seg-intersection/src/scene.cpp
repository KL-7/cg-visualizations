#include <QtGui>
#include <QDebug>

#include "scene.h"
#include "pixmapitem.h"
#include "bentleyottmann.h"

const QRect Scene::DEFAULT_SCENE_RECT = QRect(0, 0, 200, 150);
//const QRect Scene::DEFAULT_SCENE_RECT = QRect(0, 0, 1400, 850);

const int Scene::SEGMENTS_COUNT = 5;
//const int Scene::SEGMENTS_COUNT = 20000;

const int Scene::POINT_RADIUS = 2;

const double Scene::SEGMENT_LENGTH = 100;
//const double Scene::SEGMENT_LENGTH = 30;

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

//    int seed = QTime::currentTime().msec(); qDebug() << seed; qsrand(seed);
    qsrand(266);

    for (int i = 0; i < count; ++i) {
//        QPointF point(rand(width()), rand(height()));
//        QLineF segment(point, point + QPointF(SEGMENT_LENGTH, 0));
//        segment.setAngle(rand(360));
//        m_segments << segment;

        QPointF p1(rand(width()), rand(height())), p2(rand(width()), rand(height()));
        if (p1.x() > p2.x()) { QPointF tmp = p1; p1 = p2; p2 = tmp; }
        m_segments << QLineF(p1, p2);
    }
}

void Scene::regenerateSegments() {
    generateSegments();
//    addsegments();
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
    for (int i = 0; i < m_segments.size() - 1; ++i) {
        for (int j = i + 1; j < m_segments.size(); ++j) {
            if (QLineF::BoundedIntersection == m_segments[i].intersect(m_segments[j], &point)) {
                points << point;
            }
        }
    }

    m_pixmap->drawPoints(points, INTERSECTION_COLOR, POINT_RADIUS);
}

void Scene::fastColorIntersections() {
    redrawSegments();

    BentleyOttmann alg(m_segments);
    m_pixmap->drawPoints(alg.intersectionPoints(), INTERSECTION_COLOR, POINT_RADIUS);
}
