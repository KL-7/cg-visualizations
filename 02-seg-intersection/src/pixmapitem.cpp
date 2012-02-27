#include <QtGui>
#include <QtDebug>

#include "pixmapitem.h"

PixmapItem::PixmapItem(QSize size, QGraphicsScene *scene) : QGraphicsPixmapItem(0, scene), m_pixmap(new QPixmap(size)) {
    clear();
}

void PixmapItem::drawSegments(const QVector<QLineF> &segments, QColor color) {
    QPainter painter(m_pixmap);
    painter.setPen(color);
    painter.drawLines(segments);
    painter.end();
    setPixmap(*m_pixmap);
}

void PixmapItem::drawPoints(const QVector<QPointF> &points, QColor color, int radius) {
    QPainter painter(m_pixmap);
    painter.setPen(color);
    painter.setBrush(color);

    foreach(QPointF point, points) {
        painter.drawEllipse(point, radius, radius);
    }

    painter.end();
    setPixmap(*m_pixmap);
}

void PixmapItem::clear() {
    m_pixmap->fill(Qt::transparent);
    setPixmap(*m_pixmap);
}
