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

public slots:
    void findIntersection();

private:
    QVector<QLineF> m_segments;
    PixmapItem *m_pixmap;

    static const QRect DEFAULT_SCENE_RECT;
    static const QColor SEGMENT_COLOR;
    static const QColor INTERSECTION_COLOR;
    static const QColor BACKGROUND_COLOR;

    void addPolygons();
};

#endif // SCENE_H
