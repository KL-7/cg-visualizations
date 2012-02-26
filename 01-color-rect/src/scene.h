#ifndef SCENE_H
#define SCENE_H

#include <QtGui/QGraphicsScene>

class QuadraticTree;
class PointsPixmapItem;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    void recolorPoints();
    QSize rectSize() const;
    int pointsCount() const;
    void regenerate(int pointsCount, QSize rectSize);
    void switchMode(bool fast);

private:
    bool m_fast;

    QList<QPointF> m_points;
    QList<QPointF> m_pointsInsideRect;

    QGraphicsRectItem *m_rect;
    QuadraticTree *m_tree;
    PointsPixmapItem *m_pixmap;

    static const QRect DEFAULT_SCENE_RECT;
    static const QRect RECT_GEOMETRY;
    static const double POINT_RADIUS;
    static const int POINTS_COUNT;
    static const QColor INNER_POINT_COLOR;
    static const QColor OUTER_POINT_COLOR;

    double rand(double max);

    void setRectSize(QSize rectSize);
    void regeneratePoints(int count);
    void drawQuadraticTree();

    void addPoints(int count = POINTS_COUNT);
    void removePoints();
    void addRect();
};

#endif // SCENE_H
