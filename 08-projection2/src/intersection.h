#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <QtGui>

class Intersection
{
public:
    Intersection() { }

    QPolygonF findIntersection(QPolygonF polygon, QPolygonF mask);

private:
    void addPolygons();
    QPolygonF clip(QPolygonF polygon, QLineF segment);
    bool inside(QPointF point, QLineF segment);
    QPointF intersect(QLineF l1, QLineF l2);

    QPolygonF getMask(int test);
    QPolygonF getPolygon(int test);
 };

#endif // INTERSECTION_H
