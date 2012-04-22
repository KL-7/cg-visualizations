#ifndef GRAHAM_H
#define GRAHAM_H

#include <QtCore>

class Graham
{
public:
    Graham(QList<QPointF> points) : m_points(points) { }

    QList<QPointF> convexHull();

private:
    QList<QPointF> m_points;

    bool right(QPointF &p0, QPointF &p1, QPointF &p2);
};


typedef struct PointsTanComparator {
    PointsTanComparator(const QPointF &anchor) : anchor(anchor) { }

    bool operator() (const QPointF &a, const QPointF &b) {
        double tan_a = tan(a, anchor), tan_b = tan(b, anchor);
        return tan_a > tan_b || tan_a == tan_b && dist(a, anchor) > dist(b, anchor);
    }

    double tan(const QPointF &a, const QPointF &b) {
        return a.x() != b.x() ? (a.y() - b.y()) / (a.x() - b.x()) : INFINITY;
    }

    double dist(const QPointF &a, const QPointF &b) {
        QPointF d = a - b;
        return sqrt(d.x() * d.x()  + d.y() * d.y());
    }

    const QPointF anchor;
} PointsComparator;

#endif // GRAHAM_H
