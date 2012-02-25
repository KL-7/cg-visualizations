#ifndef RECT_H
#define RECT_H

#include <QtGui/QGraphicsRectItem>

class Scene;

class Rect : public QGraphicsRectItem
{
public:
    Rect(const QRectF &rect, QGraphicsItem *parent = 0, Scene *scene = 0);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // RECT_H
