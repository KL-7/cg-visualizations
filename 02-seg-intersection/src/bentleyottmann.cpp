#include "bentleyottmann.h"

bool pointComp(const Point *a, const Point *b) {
    return a->p.x() == b->p.x() ? a->rank < b->rank : a->p.x() < b->p.x();
}

QVector<QPointF> BentleyOttmann::intersectionPoints() {
    QPointF *sweepPoint = new QPointF();
    Point *tmpPoint;

    QVector<Point*> newPoints;
    QVector<QPointF> intersectionPoints;

    PointsSet points(&pointComp);
    SegmentComparator comp(sweepPoint);
    currentSegments = new SegmentsSet(comp);

    for (int i = 0; i < m_segments.size(); ++i) {
        QLineF s = m_segments[i];
        Segment *segment = new Segment(&m_segments[i], i);
        points.insert(new Point(m_segments[i].p1(), points.size(), segment, s.p1().x() <= s.p2().x()));
        points.insert(new Point(m_segments[i].p2(), points.size(), segment, s.p2().x() < s.p1().x()));
    }

    while (!points.empty()) {
        newPoints.clear();

        Point *p = *points.begin();
        points.erase(points.begin());

        sweepPoint->setX(p->p.x());
        sweepPoint->setY(p->p.y());

        if (p->s2 != 0) {
            Segment *s1 = p->s1;
            Segment *s2 = p->s2;

            if (s1->slope() > s2->slope()) {
                Segment *tmp = s1; s1 = s2; s2 = tmp;
            }

            currentSegments->erase(s1);
            currentSegments->erase(s2);

            SegmentsSet::iterator s3 = currentSegments->upper_bound(s1);

            if (s3 != currentSegments->end() && (tmpPoint = rightIntersection(s2, *s3, sweepPoint, points.size()))) {
                newPoints << tmpPoint;
            }

            SegmentsSet::iterator s4 = currentSegments->lower_bound(s2);
            s4 = (s4 != currentSegments->begin()) ? --s4 : currentSegments->end();

            if (s4 != currentSegments->end() && (tmpPoint = rightIntersection(s1, *s4, sweepPoint, points.size()))) {
                newPoints << tmpPoint;
            }

            int r1 = qMin(s1->rank, s2->rank);
            int r2 = qMax(s1->rank, s2->rank);

            s1->rank = r1;
            s2->rank = r2;

            currentSegments->insert(s1);
            currentSegments->insert(s2);
        } else if (p->left) {
            Segment *s = p->s1;

            currentSegments->insert(s);

            SegmentsSet::iterator s1 = currentSegments->upper_bound(s);

            SegmentsSet::iterator s2 = currentSegments->lower_bound(s);
            s2 = (s2 != currentSegments->begin()) ? --s2 : currentSegments->end();

            if (s1 != currentSegments->end() && (tmpPoint = rightIntersection(s, *s1, sweepPoint, points.size()))) {
                newPoints << tmpPoint;
            }

            if (s2 != currentSegments->end() && (tmpPoint = rightIntersection(s, *s2, sweepPoint, points.size()))) {
                newPoints << tmpPoint;
            }
        } else /* if (!p.left) */ {
            Segment *s = p->s1;

            SegmentsSet::iterator s1 = currentSegments->upper_bound(s);

            SegmentsSet::iterator s2 = currentSegments->lower_bound(s);
            s2 = (s2 != currentSegments->begin()) ? --s2 : currentSegments->end();

            if (s1 != currentSegments->end() && s2 != currentSegments->end() && (tmpPoint = rightIntersection(*s1, *s2, sweepPoint, points.size()))) {
                newPoints << tmpPoint;
            }

            currentSegments->erase(s);
        }

        foreach (Point *p, newPoints) {
            intersectionPoints << p->p;
            points.insert(p);
        }
    }

    return intersectionPoints;
}

Point* BentleyOttmann::rightIntersection(Segment *s1, Segment *s2, const QPointF *p, int rank) {
    QPointF intersectionPoint;
    QLineF::IntersectType intersecitonType = s1->s->intersect(*s2->s, &intersectionPoint);

    if (intersecitonType == QLineF::BoundedIntersection && intersectionPoint.x() > p->x()) {
        return new Point(intersectionPoint, rank, s1, false, s2);
    } else {
        return 0;
    }
}
