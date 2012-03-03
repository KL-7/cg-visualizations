#include "bentleyottmann.h"

bool pointComp(const Point *a, const Point *b) {
    return a->p->x() < b->p->x();
}

QVector<QPointF> BentleyOttmann::intersectionPoints() {
    QPointF *sweepPoint = new QPointF();
    Point *tmpPoint;

    QVector<Point*> newPoints;
    QVector<QPointF> intersectionPoints;

    PointsSet points(&pointComp);
    SegmentComparator comp(sweepPoint);
    SegmentsSet currentSegments(comp);
    map<QLineF*, int> ranks;

    foreach (QLineF segment, m_segments) {
        points.insert(new Point(&segment.p1(), &segment, segment.p1().x() <= segment.p2().x()));
        points.insert(new Point(&segment.p2(), &segment, segment.p2().x() < segment.p1().x()));
    }

    int currentRank = 0;

    while (!points.empty()) {
        newPoints.clear();
        Point *p = *points.begin();
        points.erase(points.begin());
        sweepPoint->setX(p->p->x());
        sweepPoint->setY(p->p->y());

        if (p->s2 != 0) {

        } else if (p->left) {
            int rank = currentRank++;
            ranks[p->s1] = rank;
            Segment *s = new Segment(p->s1, rank);

            currentSegments.insert(s);

            SegmentsSet::iterator s1 = currentSegments.upper_bound(s);
            SegmentsSet::iterator s2 = ++currentSegments.lower_bound(s);

            if (s1 != currentSegments.end() && (tmpPoint = rightIntersection(s, *s1, sweepPoint))) {
                newPoints << tmpPoint;
            }

            if (s2 != currentSegments.end() && (tmpPoint = rightIntersection(s, *s2, sweepPoint))) {
                newPoints << tmpPoint;
            }
        } else /* if (!p.left) */ {
            Segment *s = new Segment(p->s1, ranks[p->s1]);

            SegmentsSet::iterator s1 = currentSegments.upper_bound(s);
            SegmentsSet::iterator s2 = ++currentSegments.lower_bound(s);

            if (s1 != currentSegments.end() && s2 != currentSegments.end() && (tmpPoint = rightIntersection(*s1, *s2, sweepPoint))) {
                newPoints << tmpPoint;
            }

            currentSegments.erase(s);
        }
    }

    return intersectionPoints;
}

Point* BentleyOttmann::rightIntersection(Segment *s1, Segment *s2, const QPointF *p) {
    return 0;
}
