#include <QtGui>

#include "pointspixmapitem.h"

PointsPixmapItem::PointsPixmapItem(QSize size, QGraphicsScene *scene) : QGraphicsPixmapItem(0, scene) {
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter p(&pixmap);
    setPixmap(pixmap);
}

void PointsPixmapItem::drawPoints(const QList<QPointF> &points, QColor color) {
    QPixmap pixmapCopy = pixmap();
    QPainter painter(&pixmapCopy);
    painter.setPen(color);

    foreach (QPointF point, points) {
        painter.drawPoint(point);
    }

    painter.end();
    setPixmap(pixmapCopy);
}
