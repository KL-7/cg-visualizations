#include "intersection.h"

void Intersection::addPolygons() { }

QPolygonF Intersection::findIntersection(QPolygonF polygon, QPolygonF mask) {
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

QPolygonF Intersection::getPolygon(int test) {
    QPolygonF p;

    switch (test) {
    case 1:
        p << QPointF(200, 200)
          << QPointF(300, 100)
          << QPointF(400, 150)
          << QPointF(350, 250)
          << QPointF(450, 300)
          << QPointF(400, 400)
          << QPointF(300, 450);
        break;
    case 2:
        p << QPointF(400, 250)
          << QPointF(410, 260)
          << QPointF(390, 260);
        break;
    case 3:
        p << QPointF(400, 350)
          << QPointF(410, 360)
          << QPointF(390, 360);
        break;
    case 4:
        p << QPointF(10, 10)
          << QPointF(800, 250)
          << QPointF(10, 570);
        break;
    case 5:
        p << QPointF(10, 210)
          << QPointF(300, 210)
          << QPointF(300, 240)
          << QPointF(20, 240)
          << QPointF(20, 260)
          << QPointF(300, 260)
          << QPointF(300, 280)
          << QPointF(10, 280);
        break;
    }

    return p;
}

QPolygonF Intersection::getMask(int test) {
    QPolygonF p;

    switch (test) {
        case 1:
            p << QPointF(100, 150)
              << QPointF(500, 200)
              << QPointF(480, 320)
              << QPointF(320, 350);
        break;
    }

    return p;
}
