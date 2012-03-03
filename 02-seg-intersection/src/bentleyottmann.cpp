#include "bentleyottmann.h"

QVector<QPointF> BentleyOttmann::intersectionPoints() {
//    PointsSet points(&pointComp);
    set<QLineF> currentSegments;
    QVector<QPointF> intersectionPoints;

//    foreach (QLineF segment, m_segments) {
//        points.insert(segment.p1());
//        points.insert(segment.p1());
//    }

    return intersectionPoints;
}

bool BentleyOttmann::pointComp(const Point &a, const Point &b) {
    return a.point->x() < b.point->x();
}
