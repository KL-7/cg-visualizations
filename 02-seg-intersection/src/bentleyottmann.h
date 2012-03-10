#ifndef BENTLEYOTTMANN_H
#define BENTLEYOTTMANN_H

#include <QtCore>
#include <set>
#include <map>

using namespace std;


struct Segment {
    QLineF *s;
    int rank;

    Segment(QLineF *segment, int rank) : s(segment), rank(rank) { }

    qreal high(const QPointF* p) const {
        QPointF intersectionPoint;
        s->intersect(QLineF(*p, *p + QPointF(0, 1)), &intersectionPoint);
//        qDebug() << "hight: " << *s << ", " << *p << " - " << intersectionPoint.y();
        return intersectionPoint.y();
//        return s->p1().y() + (s->p2().y() - s->p1().y()) / (s->p2().x() - s->p1().x()) * (p->x() - s->p1().x());
    }

    qreal slope() const {
        return (s->p2().y() - s->p1().y()) / (s->p2().x() - s->p1().x());
    }
} typedef Segment;


struct Point {
    QPointF p;
    int rank;
    Segment *s1;
    Segment *s2;
    bool left;

    Point(QPointF point, int rank, Segment *segment1, bool isLeftEnd, Segment *segment2 = 0)
        : p(point), rank(rank), s1(segment1), s2(segment2), left(isLeftEnd) { }
} typedef Point;


QDebug operator<<(QDebug dbg, const Segment &s);
QDebug operator<<(QDebug dbg, const Point &p);


struct SegmentComparator {
    const QPointF *sweepPoint;

    static const double EPS = 1e-5;

    SegmentComparator(const QPointF *sweepPoint) : sweepPoint(sweepPoint) { }

    bool operator() (const Segment *a, const Segment *b) { 
        double ah = a->high(sweepPoint);
        double bh = b->high(sweepPoint);

        bool result = a != b && (qAbs(ah - bh) <= EPS ? a->rank < b->rank : ah < bh);

        qDebug() << "\n  # comp:" << *a << (result ? " < " : " >= ") << *b
                 << "\n        : ah - " << ah << ", bh - " << bh << ", sweepPoint - " << *sweepPoint;

        return result;
    }
} typedef SegmentComparator;


typedef set<Point*, bool (*) (const Point*, const Point*)> PointsSet;
typedef set<Segment*, SegmentComparator> SegmentsSet;


class BentleyOttmann
{
public:
    BentleyOttmann(const QVector<QLineF> &segments) : m_segments(segments) { }
    QVector<QPointF> intersectionPoints();

private:
    QVector<QLineF> m_segments;
    SegmentsSet *currentSegments;

    Point* rightIntersection(Segment *s1, Segment *s2, const QPointF *p, int rank);
    void printCurrentSegments();
};

#endif // BENTLEYOTTMANN_H
