#include "bentleyottmann.h"

bool pointComp(const Point &a, const Point &b) {
    return a.point->x() < b.point->x();
}

QVector<QPointF> BentleyOttmann::intersectionPoints() {
    PointsSet points(&pointComp);
    set<QLineF> currentSegments;
    QVector<QPointF> intersectionPoints;

    foreach (QLineF segment, m_segments) {
        points.insert(Point(&segment.p1(), &segment, segment.p1().x() <= segment.p2().x()));
        points.insert(Point(&segment.p2(), &segment, segment.p2().x() < segment.p1().x()));
    }

    while (!points.empty()) {
        Point p = points.
    }

    return intersectionPoints;
}

