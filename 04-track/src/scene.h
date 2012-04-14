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
    static const double TRACK_RADIUS;

    void setup();
    void addPart(MovableItemGroup *part);
    void moveTrack(double step);
    void addWheel(QPointF pos, double radius = TRACK_RADIUS);
    void addTrack(QPointF pos, double width = TRACK_WIDTH, double radius = TRACK_RADIUS);
 };

#endif // SCENE_H
