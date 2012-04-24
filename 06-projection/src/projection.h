#ifndef PROJECTION_H
#define PROJECTION_H

#include <QtGui>

typedef QPair<QVector3D, QVector3D> Segment3D;

class Projection
{
public:
    Projection();

    void render(QGraphicsScene *scene);

    QMatrix4x4 transformMatrix();
    QVector3D transformPoint3D(QVector3D p, QMatrix4x4);
    QPointF transformPoint(QVector3D p, QMatrix4x4);

private:
    QVector3D vrp;
    QVector3D cop;

    QVector3D vpn;
    QVector3D vup;

    QPointF uvMin;
    QPointF uvMax;

    double ff;
    double bb;

    QMatrix4x4 shiftMatrix(double dx, double dy, double dz);
    QMatrix4x4 scaleMatrix(double sx, double sy, double sz);

    QList<Segment3D> unitCube();
    Segment3D segment3D(double x1, double y1, double z1, double x2, double y2, double z2);
};

#endif // PROJECTION_H
