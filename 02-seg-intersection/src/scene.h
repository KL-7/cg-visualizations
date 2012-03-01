#ifndef SCENE_H
#define SCENE_H

#include <QtGui/QGraphicsScene>

class QuadraticTree;
class PixmapItem;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    void regenerate(int pointsCount, QSize rectSize);

public slots:
    void colorIntersections();
    void fastColorIntersections();

private:
    QVector<QLineF> m_segments;
    PixmapItem *m_pixmap;

    static const QRect DEFAULT_SCENE_RECT;
    static const int SEGMENTS_COUNT;
    static const double SEGMENT_LENGTH;
    static const QColor SEGMENT_COLOR;
    static const QColor INTERSECTION_COLOR;
    static const int POINT_RADIUS;

    double rand(double max);

    void generateSegments(int count = SEGMENTS_COUNT);
};

#endif // SCENE_H
