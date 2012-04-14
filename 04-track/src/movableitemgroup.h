#ifndef MOVABLEITEMGROUP_H
#define MOVABLEITEMGROUP_H

#include <QGraphicsItem>
#include <QtDebug>

class MovableItemGroup : public QGraphicsItemGroup
{
public:
    virtual void move(double step) = 0;
};

#endif // MOVABLEITEMGROUP_H
