#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include <QGraphicsPixmapItem>

class PixmapItem : public QGraphicsPixmapItem
{
public:
    PixmapItem(QSize size, QGraphicsScene *scene = 0);
    void drawPoints(const QList<QPointF> &points, QColor color = Qt::black);
    void drawLines(const QList<QLineF> &lines, QColor color = Qt::red);
    void clear();

private:
    QSize m_size;
    static const int PEN_RADIUS;
};

#endif // PIXMAPITEM_H
