#ifndef SCENE_H
#define SCENE_H

#include <QtGui/QGraphicsScene>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);

public slots:
    void colorIntersection();

private:
    QGraphicsPolygonItem *polygon, *mask, *intersection;

    static const QRect DEFAULT_SCENE_RECT;
    static const QColor POLYGON_COLOR;
    static const QColor MASK_COLOR;
    static const QColor INTERSECTION_COLOR;
    static const QColor BACKGROUND_COLOR;

    void addPolygons();
    QPolygonF findIntersection(QPolygonF polygon, QPolygonF mask);
    QPolygonF clip(QPolygonF polygon, QLineF segment);
    bool inside(QPointF point, QLineF segment);
    QPointF intersect(QLineF l1, QLineF l2);

    QPolygonF getMask(int test);
    QPolygonF getPolygon(int test);
 };

#endif // SCENE_H
