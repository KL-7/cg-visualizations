#include "bentleyottmann.h"

bool pointComp(const Point *a, const Point *b) {
    return a->p.x() == b->p.x() ? a->rank < b->rank : a->p.x() < b->p.x();
}

QDebug operator<<(QDebug dbg, const Segment &s) {
    dbg.nospace() << "Segment(" << s.rank << ", " << *s.s << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const Point &p) {
    dbg.nospace() << "Points(" << p.p << ", " << (p.left ? "left" : "right") << ", " << (p.s2 != 0 ? "intersection" : "") << ")";
    return dbg.space();
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
        currentSegments->insert(segment);
//        qDebug() << "add points of " << *segment->s;
        points.insert(new Point(m_segments[i].p1(), points.size(), segment, s.p1().x() <= s.p2().x()));
        points.insert(new Point(m_segments[i].p2(), points.size(), segment, s.p2().x() < s.p1().x()));
    }

//    qDebug() << "total segments size: " << m_segments.size();
//    qDebug() << "points size: " << points.size();

//    for (SegmentsSet::iterator it = currentSegments->begin(); it != currentSegments->end(); ++it) {
//        qDebug() << "find " << *(*it)->s << " - " << (currentSegments->find(*it) != currentSegments->end());
//    }
    currentSegments->clear();

    while (!points.empty()) {
        qDebug() << "\n\n\n=================";
        qDebug() << "segments size: " << currentSegments->size();

        newPoints.clear();

        Point *p = *points.begin();
        points.erase(points.begin());

        qDebug() << "point: " << p->p;

        sweepPoint->setX(p->p.x());
        sweepPoint->setY(p->p.y());

        if (p->s2 != 0) {
            Segment *s1 = p->s1;
            Segment *s2 = p->s2;

            if (s1->slope() > s2->slope()) {
                Segment *tmp = s1; s1 = s2; s2 = tmp;
            }

            qDebug() << "=> intersection: " << *s1 << " & " << *s2;

            qDebug() << "---- extract";
            currentSegments->erase(s1);
            currentSegments->erase(s2);

            SegmentsSet::iterator s3 = currentSegments->upper_bound(s1);

            if (s3 != currentSegments->end()) { qDebug() << "s3: " << **s3; } else { qDebug() << "s3: end"; }

            if (s3 != currentSegments->end() && (tmpPoint = rightIntersection(s2, *s3, sweepPoint, points.size()))) {
                qDebug() << "add point: " << *tmpPoint;
                newPoints << tmpPoint;
            }

//            qDebug() << "!!!!!!!  s2 - " << *s2;
            SegmentsSet::iterator s4 = currentSegments->lower_bound(s2);
//            qDebug() << "!!!!!!!  s4 - " << **s4;
            s4 = (s4 != currentSegments->begin()) ? --s4 : currentSegments->end();
//            qDebug() << "!!!!!!!  s4 - " << **s4 << " (updated)";

            if (s4 != currentSegments->end()) { qDebug() << "s4: " << **s4; } else { qDebug() << "s4: end"; }

            if (s4 != currentSegments->end() && (tmpPoint = rightIntersection(s1, *s4, sweepPoint, points.size()))) {
                qDebug() << "add point: " << *tmpPoint;
                newPoints << tmpPoint;
            }

            printCurrentSegments();

            int r1 = qMin(s1->rank, s2->rank);
            int r2 = qMax(s1->rank, s2->rank);

            s1->rank = r1;
            s2->rank = r2;

            qDebug() << "---- swapped; insert";
            currentSegments->insert(s1);
            currentSegments->insert(s2);

            printCurrentSegments();
        } else if (p->left) {
            qDebug() << "=> left";
            Segment *s = p->s1;

            qDebug() << "insert " << *s->s;
            currentSegments->insert(s);
            qDebug() << "segments size: " << currentSegments->size();

            printCurrentSegments();

            SegmentsSet::iterator s1 = currentSegments->upper_bound(s);

            if (s1 != currentSegments->end()) { qDebug() << "s1: " << **s1; } else { qDebug() << "s1: end"; }

            SegmentsSet::iterator s2 = currentSegments->lower_bound(s);
            s2 = (s2 != currentSegments->begin()) ? --s2 : currentSegments->end();

            if (s2 != currentSegments->end()) { qDebug() << "s2: " << **s2; } else { qDebug() << "s2: end"; }

            if (s1 != currentSegments->end() && (tmpPoint = rightIntersection(s, *s1, sweepPoint, points.size()))) {
                newPoints << tmpPoint;
            }

            if (s2 != currentSegments->end() && (tmpPoint = rightIntersection(s, *s2, sweepPoint, points.size()))) {
                newPoints << tmpPoint;
            }
        } else /* if (!p.left) */ {
            qDebug() << "=> right";
            Segment *s = p->s1;

            qDebug() << "segment - " << *s;
            printCurrentSegments();
//            qDebug() << (currentSegments->find(s) != currentSegments->end());

            SegmentsSet::iterator s1 = currentSegments->upper_bound(s);

            SegmentsSet::iterator s2 = currentSegments->lower_bound(s);
            s2 = (s2 != currentSegments->begin()) ? --s2 : currentSegments->end();

            if (s1 != currentSegments->end() && s2 != currentSegments->end() && (tmpPoint = rightIntersection(*s1, *s2, sweepPoint, points.size()))) {
                newPoints << tmpPoint;
            }

            qDebug() << ">>> find: " << (currentSegments->find(s) != currentSegments->end());

            printCurrentSegments();

//            qDebug() << ">>> " << *(--currentSegments->end()) << ", " << s;
//            qDebug() << ">>> " << comp(*(--currentSegments->end()), s) << ", " << comp(s, *(--currentSegments->end()));

            qDebug() << "\n>>> erase";
            currentSegments->erase(s);
            qDebug() << ">>> " << currentSegments->size() << " segments";

            printCurrentSegments();
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

    qDebug() << "\n >->  intersection of " << *s1 << " & " << *s2 << " (point - " << *p << "): " << intersecitonType << ", " << intersectionPoint;

    if (intersecitonType == QLineF::BoundedIntersection && intersectionPoint.x() > p->x()) {
        return new Point(intersectionPoint, rank, s1, false, s2);
    } else {
        return 0;
    }
}

void BentleyOttmann::printCurrentSegments() {
    qDebug() << "";
    for (SegmentsSet::iterator it = currentSegments->begin(); it != currentSegments->end(); ++it) {
        qDebug() << "--> " << **it;
    }
}
