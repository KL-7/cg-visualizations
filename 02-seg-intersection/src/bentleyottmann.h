#ifndef BENTLEYOTTMANN_H
#define BENTLEYOTTMANN_H

#include <QtCore>
#include <set>

using namespace std;

struct Point {
    QPointF *point;
    QLineF *segment;
    bool isLeftEnd;

    Point(QPointF *point, QLineF *segment, bool isLeftEnd) : point(point), segment(segment), isLeftEnd(isLeftEnd) { }
} typedef Point;

typedef set<Point, bool (*) (const Point&, const Point&)> PointsSet;

class BentleyOttmann
{
public:
    BentleyOttmann(const QVector<QLineF> &segments) : m_segments(segments) { }
    QVector<QPointF> intersectionPoints();

private:
    QVector<QLineF> m_segments;
};

#endif // BENTLEYOTTMANN_H
