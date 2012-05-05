#include "intersection.h"

QPolygonF Intersection::calculate(QPolygonF polygon, QPolygonF mask) {
    if (!inside(mask[2], QLineF(mask[0], mask[1]))) {
        QPolygonF m;
        for (int i = mask.size() - 1; i >=0 ; --i) {
            m << mask[i];
        }
        mask = m;
    }

    for (int i = 0; i < mask.size(); ++i) {
        polygon = clip(polygon, QLineF(mask[i], mask[(i + 1) % mask.size()]));
    }

    return polygon;
}

QPolygonF Intersection::clip(QPolygonF polygon, QLineF segment) {
    QPolygonF result;

    for (int i = 0; i < polygon.size(); ++i) {
        QLineF s(polygon[i], polygon[(i + 1) % polygon.size()]);
        if (inside(s.p1(), segment)) {
            if (inside(s.p2(), segment)) {
                result << s.p2();
            } else {
                result << intersect(segment, s);
            }
        } else {
            if (inside(s.p2(), segment)) {
                result << intersect(segment, s) << s.p2();
            }
        }
    }

    return result;
}

bool Intersection::inside(QPointF point, QLineF segment) {
    QLineF s(segment.p1(), point);
    return segment.dx() * s.dy() - s.dx() * segment.dy() > 0;
}

QPointF Intersection::intersect(QLineF l1, QLineF l2) {
    QPointF p;
    l1.intersect(l2, &p);
    return p;
}
