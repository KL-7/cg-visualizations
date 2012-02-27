#ifndef POINTSPIXMAPITEM_H
#define POINTSPIXMAPITEM_H

#include <QGraphicsPixmapItem>

class PixmapItem : public QGraphicsPixmapItem
{
public:
    PixmapItem(QSize size, QGraphicsScene *scene = 0);
    void drawSegments(const QVector<QLineF> &segments, QColor color);
    void drawPoints(const QVector<QPointF> &points, QColor color, int radius);
    void clear();

private:
    QPixmap *m_pixmap;
};

#endif // POINTSPIXMAPITEM_H
