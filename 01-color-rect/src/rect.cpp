#include "rect.h"
#include "scene.h"
#include <QtGui>

Rect::Rect(const QRectF &rect, QGraphicsItem *parent, Scene *scene)
    : QGraphicsRectItem(rect, parent, scene)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    setPen(QPen(Qt::black, 2));
    setBrush(QColor("#8CB0F5"));
}

QVariant Rect::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        ((Scene*)scene())->recolorPoints();
    }

    return QGraphicsRectItem::itemChange(change, value);
}
