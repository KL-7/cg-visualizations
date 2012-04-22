#ifndef SCENE_H
#define SCENE_H

#include <QtGui/QGraphicsScene>

class PixmapItem;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);

public slots:
    void generatePoints(int pointsNumber = POINTS_NUMBER);
    void buildConvexHull();

private:
    static const QRect DEFAULT_SCENE_RECT;
    static const QColor BACKGROUND_COLOR;
    static const int POINTS_RADIUS;
    static const int POINTS_NUMBER;
    static const int PADDING;

    QList<QPointF> m_points;

    PixmapItem *m_pixmap;
    double rand(double range);
};

#endif // SCENE_H
