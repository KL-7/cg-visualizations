#ifndef SCENE_H
#define SCENE_H

#include <QtGui/QGraphicsScene>

class MovableItemGroup;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QVector<MovableItemGroup*> m_trackParts;

    static const QRect DEFAULT_SCENE_RECT;
    static const QColor BACKGROUND_COLOR;
    static const int TRACK_STEP;
    static const QPointF INITIAL_POS;
    static const double TRACK_WIDTH;

    void setup();
    void moveTrack(double step);
    void addWheel(QPointF pos);
 };

#endif // SCENE_H
