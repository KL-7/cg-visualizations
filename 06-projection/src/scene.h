#ifndef SCENE_H
#define SCENE_H

#include <QtGui/QGraphicsScene>

class PixmapItem;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);

private:
    static const QRect DEFAULT_SCENE_RECT;
    static const QColor BACKGROUND_COLOR;

    PixmapItem *m_pixmap;
};

#endif // SCENE_H
