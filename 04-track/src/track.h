#ifndef TRACK_H
#define TRACK_H

#include "movableitemgroup.h"

class Track : public MovableItemGroup
{
public:
    Track(double width, double radius);
    void move(double step);

    double width() const { return m_width; }
    double height() const { return 2 * m_radius; }
    double radius() const { return m_radius; }

private:
    double m_width;
    double m_radius;
    QVector<QGraphicsItem*> m_points;

    static const int POINTS_NUMBER;
    static const double POINTS_RAIDUS;

    void addPoints();
    void addPoint(QPointF point);
    QPointF movePoint(QPointF point, double step);
    void movePoint(QGraphicsItem *point, double step);
    QPointF rotatePoint(QPointF pivot, QPointF point, double angle);
};

#endif // TRACK_H
