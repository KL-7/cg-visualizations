#ifndef SCENE_H
#define SCENE_H

#include <QtGui/QGraphicsScene>

class Projection;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    static const QRect DEFAULT_SCENE_RECT;
    static const QColor BACKGROUND_COLOR;

    Projection *projection;
};

#endif // SCENE_H
