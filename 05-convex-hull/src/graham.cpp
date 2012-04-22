#include <algorithm>

#include "graham.h"

QList<QPointF> Graham::convexHull() {
    QQueue<QPointF> *queue = new QQueue<QPointF>();
    QList<QPointF> *points = static_cast<QList<QPointF>*>(queue);

    foreach (QPointF p, m_points) {
        points->append(p);
    }

    QPointF leftPoint = points->first();
    foreach (QPointF p, *points) {
        if (p.x() < leftPoint.x() || p.x() == leftPoint.x() && p.y() > leftPoint.y()) {
            leftPoint = p;
        }
    }

    points->removeOne(leftPoint);
    std::sort(points->begin(), points->end(), PointsTanComparator(leftPoint));
    points->append(leftPoint);

    QList<QPointF> hull;
    hull.append(leftPoint);
    QPointF a = queue->dequeue(), b = queue->dequeue();

    while (true) {
        if (right(hull.last(), a, b)) {
            a = hull.takeLast();
        } else {
            hull.append(a);
            a = b;
            if (queue->empty()) {
                break;
            } else {
                b = queue->dequeue();
            }
        }
    }

    return hull;
}

bool Graham::right(QPointF &p0, QPointF &p1, QPointF &p2) {
    return (p1.x() - p0.x()) * (p2.y() - p0.y()) - (p2.x() - p0.x()) * (p1.y() - p0.y()) > 0;
}
