#include <QtGui>

#include "pointspixmapitem.h"

PointsPixmapItem::PointsPixmapItem(QSize size, QGraphicsScene *scene) : QGraphicsPixmapItem(0, scene), m_size(size) {
    clear();
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

void PointsPixmapItem::clear() {
    QPixmap pixmap(m_size);
    pixmap.fill(Qt::transparent);
    setPixmap(pixmap);
}
