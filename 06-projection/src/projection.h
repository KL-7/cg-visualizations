#ifndef PROJECTION_H
#define PROJECTION_H

#include <QtGui>

#include "point3d.h"
#include "vector3d.h"

class Projection
{
public:
    Projection();

    QPixmap render(QSize size);

    QVector3D transformPoint(QVector3D p, QMatrix4x4);
    QMatrix4x4 transformMatrix();

private:
    QVector3D vrp;
    QVector3D cop;

    QVector3D vpn;
    QVector3D vup;

    QPointF uvMin;
    QPointF uvMax;

    double ff;
    double bb;
};

#endif // PROJECTION_H
