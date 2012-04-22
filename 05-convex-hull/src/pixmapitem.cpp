#include <QtGui>

#include "pixmapitem.h"

const int PixmapItem::PEN_RADIUS = 2;

PixmapItem::PixmapItem(QSize size, QGraphicsScene *scene) : QGraphicsPixmapItem(0, scene), m_size(size) {
    clear();
}

void PixmapItem::drawPoints(const QList<QPointF> &points, QColor color) {
    QPixmap pixmapCopy = pixmap();
    QPainter painter(&pixmapCopy);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, PEN_RADIUS));

    foreach (QPointF point, points) {
        painter.drawPoint(point);
    }

    painter.end();
    setPixmap(pixmapCopy);
}

void PixmapItem::drawLines(const QList<QLineF> &lines, QColor color) {
    QPixmap pixmapCopy = pixmap();
    QPainter painter(&pixmapCopy);
    painter.device();

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, PEN_RADIUS));

    foreach (QLineF line, lines) {
        painter.drawLine(line);
    }

    painter.end();
    setPixmap(pixmapCopy);
}

void PixmapItem::clear() {
    QPixmap pixmap(m_size);
    pixmap.fill(Qt::transparent);
    setPixmap(pixmap);
}
