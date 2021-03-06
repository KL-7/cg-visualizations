#ifndef PROJECTION_H
#define PROJECTION_H

#include <QtGui>

typedef QPair<QVector3D, QVector3D> Segment3D;

class Projection
{
public:
    Projection();

    void render(QGraphicsScene *scene);
    void move(double horizontal, double vertical);
    void rotate(double horizontal, double vertical);
    void zoom(double factor);

private:
    static const double MOVE_SCALE;
    static const double ROTATE_SCALE;
    static const double ZOOM_SCALE;

    QVector3D cop;

    QVector3D vpn;
    QVector3D vup;

    double ff;
    double bb;

    double vrpDistance;
    double viewWindowWidth;
    double viewWindowHeight;

    QVector3D vrp() { return cop + vrpDistance * vpn.normalized(); }
    QPointF uvMin() { return QPointF(-viewWindowWidth / 2, -viewWindowHeight / 2); }
    QPointF uvMax() { return QPointF(viewWindowWidth / 2, viewWindowHeight / 2); }

    QMatrix4x4 scaleMatrix(double sx, double sy, double sz);
    QMatrix4x4 shiftMatrix(double dx, double dy, double dz);
    QMatrix4x4 shiftMatrix(QVector3D v);

    QMatrix4x4 transformMatrix();
    QVector3D transformPoint3D(QVector3D p, QMatrix4x4);
    QPointF transformPoint(QVector3D p, QMatrix4x4);

    QList<Segment3D> unitCube();
    QList<Segment3D> axes();
    Segment3D segment3D(double x1, double y1, double z1, double x2, double y2, double z2);
    void renderSegments(QGraphicsScene *scene, const QList<Segment3D> &segments, const QMatrix4x4 &matrix);
};

#endif // PROJECTION_H
