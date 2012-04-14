#include <QtGui>

#include "wheel.h"

const int Wheel::SPOKES_NUMBER = 3;
const int Wheel::DEGREES_IN_PI = 180;

Wheel::Wheel(double radius) : m_radius(radius) {
    QGraphicsEllipseItem *wheel = new QGraphicsEllipseItem(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius, this);
    wheel->setBrush(Qt::lightGray);
    addToGroup(wheel);

    for (int i = 0; i < SPOKES_NUMBER; ++i) {
        QGraphicsLineItem *item = new QGraphicsLineItem(0, -m_radius, 0, m_radius, this);
        item->rotate(2 * DEGREES_IN_PI * i / SPOKES_NUMBER);
        addToGroup(item);
    }
}

void Wheel::move(double step) {
    moveBy(step, 0);
    setTransform(QTransform().rotateRadians(step / m_radius), true);
}
