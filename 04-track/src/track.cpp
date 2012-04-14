#include <cmath>
#include <QtGui>

#include "track.h"

static const double PI = 4 * atan(1);

const int Track::POINTS_NUMBER = 50;
const double Track::POINTS_RAIDUS = 3;

Track::Track(double width, double radius) : m_width(width), m_radius(radius) {
    addPoints();
}

void Track::addPoints() {
    double step = (2 * PI * m_radius + 2 * m_width) / POINTS_NUMBER;

    QPointF p(-m_width / 2, -m_radius);

    for (int i = 0; i < POINTS_NUMBER; ++i) {
        addPoint(p);
        p = movePoint(p, step);
    }
}

void Track::addPoint(QPointF point) {
    QGraphicsEllipseItem *p = new QGraphicsEllipseItem(
            -POINTS_RAIDUS,
            -POINTS_RAIDUS,
            2 * POINTS_RAIDUS,
            2 * POINTS_RAIDUS,
            this
    );

    p->setBrush(Qt::yellow);

    m_points << p;
    addToGroup(p);

    p->setPos(point);
}

void Track::move(double step) {
    moveBy(step, 0);

    foreach (QGraphicsItem *point, m_points) {
        movePoint(point, step);
    }
}

void Track::movePoint(QGraphicsItem *point, double step) {
    point->setPos(movePoint(point->pos(), step));
}

QPointF Track::movePoint(QPointF point, double step) {
    if (-m_width / 2 <= point.x() && point.x() <= m_width / 2) {
        if (point.y() >= 0) {
            point.rx() -= step;
            point.setY(m_radius);
        } else {
            point.rx() += step;
            point.setY(-m_radius);
        }
    } else {
        QPointF pivot(point.x() > 0 ? m_width / 2 : -m_width / 2, 0);
        point = rotatePoint(pivot, point, step / m_radius);
    }

    return point;
}

QPointF Track::rotatePoint(QPointF pivot, QPointF point, double angle) {
    QPointF dp = point - pivot;
    return point - dp + QTransform().rotateRadians(angle).map(dp);
}
