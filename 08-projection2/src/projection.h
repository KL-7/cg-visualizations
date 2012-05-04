#ifndef PROJECTION_H
#define PROJECTION_H

#include <QtGui>

#include "facet3d.h"

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

    Point3D cop;

    Point3D vpn;
    Point3D vup;

    double ff;
    double bb;

    double vrpDistance;
    double viewWindowWidth;
    double viewWindowHeight;

    Point3D vrp() { return cop + vrpDistance * vpn.normalized(); }
    QPointF uvMin() { return QPointF(-viewWindowWidth / 2, -viewWindowHeight / 2); }
    QPointF uvMax() { return QPointF(viewWindowWidth / 2, viewWindowHeight / 2); }

    QMatrix4x4 scaleMatrix(double sx, double sy, double sz);
    QMatrix4x4 shiftMatrix(double dx, double dy, double dz);
    QMatrix4x4 shiftMatrix(Point3D v);

    QMatrix4x4 transformMatrix();
    Point3D transformPoint3D(Point3D p, QMatrix4x4);
    QPointF transformPoint(Point3D p, QMatrix4x4);
    Facet3D transformFacet3D(Facet3D facet, QMatrix4x4 m);

    QList<Facet3D> figure();
    QList<Segment3D> unitCube();
    QList<Segment3D> axes();
    Segment3D segment3D(double x1, double y1, double z1, double x2, double y2, double z2);
    void renderFigure(QGraphicsScene *scene, const QList<Facet3D> &facets, const QMatrix4x4 &matrix);
    void renderSegments(QGraphicsScene *scene, const QList<Segment3D> &segments, const QMatrix4x4 &matrix);
};

#endif // PROJECTION_H
