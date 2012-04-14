#ifndef WHEEL_H
#define WHEEL_H

#include "movableitemgroup.h"

class Wheel : public MovableItemGroup
{
public:
    Wheel(double radius = RADIUS);

    double radius() const { return m_radius; }

    void move(double step);
private:
    double m_radius;

    static const double RADIUS;
    static const int SPOKES_NUMBER;
};

#endif // WHEEL_H
