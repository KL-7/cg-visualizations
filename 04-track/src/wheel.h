#ifndef WHEEL_H
#define WHEEL_H

#include "movableitemgroup.h"

class Wheel : public MovableItemGroup
{
public:
    Wheel(double radius);

    double radius() const { return m_radius; }

    void move(double step);
private:
    double m_radius;

    static const int SPOKES_NUMBER;
    static const int DEGREES_IN_PI;
};

#endif // WHEEL_H
