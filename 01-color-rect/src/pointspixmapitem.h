#ifndef POINTSPIXMAPITEM_H
#define POINTSPIXMAPITEM_H

#include <QGraphicsPixmapItem>

class PointsPixmapItem : public QGraphicsPixmapItem
{
public:
    PointsPixmapItem(QSize size, QGraphicsScene *scene = 0);
    void drawPoints(const QList<QPointF> &points, QColor color);
};

#endif // POINTSPIXMAPITEM_H
