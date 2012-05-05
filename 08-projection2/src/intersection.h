#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <QtGui>

class Intersection
{
public:
    static QPolygonF calculate(QPolygonF polygon, QPolygonF mask);

private:
    static QPolygonF clip(QPolygonF polygon, QLineF segment);
    static bool inside(QPointF point, QLineF segment);
    static QPointF intersect(QLineF l1, QLineF l2);
};

#endif // INTERSECTION_H
