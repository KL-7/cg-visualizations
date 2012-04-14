#include <QtDebug>
#include <cmath>

#include "wheel.h"

static const double PI = 4 * atan(1);
static const double DEGREES_IN_PI = 180;

const double Wheel::RADIUS = 50;
const int Wheel::SPOKES_NUMBER = 3;

Wheel::Wheel(double radius) : m_radius(radius) {
    addToGroup(new QGraphicsEllipseItem(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius, this));

    for (int i = 0; i < SPOKES_NUMBER; ++i) {
        QGraphicsItem *item = new QGraphicsLineItem(0, -m_radius, 0, m_radius, this);
        item->rotate(2 * DEGREES_IN_PI * i / SPOKES_NUMBER);
        addToGroup(item);
    }
    setTransformOriginPoint(m_radius, m_radius);
}

void Wheel::move(double step) {
    moveBy(step, 0);
    setTransform(QTransform().rotate(DEGREES_IN_PI * step / (PI * m_radius)), true);
}
