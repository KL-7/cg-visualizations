#ifndef PROJECTION_H
#define PROJECTION_H

#include <QtGui>

#include "facet3d.h"

class Projection
{
public:
    Projection(QGraphicsScene *scene);

    void render();
    void move(double horizontal, double vertical);
    void rotate(double horizontal, double vertical);
    void zoom(double factor);

private:
    static const double MOVE_SCALE;
    static const double ROTATE_SCALE;
    static const double ZOOM_SCALE;
    static const double PIXELIZATION_LIMIT;

    enum FacetType { OUTER = 1, ENCLOSING, INTERSECTING, INNER };

    QGraphicsScene *m_scene;
    QList<Facet3D*> m_figure;

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
    Facet3D* transformFacet3D(Facet3D *facet, QMatrix4x4 m);

    void warnock(const QRectF &rect, QList<Facet3D*> enclosingFacets, QList<Facet3D*> otherFacets);

    void setupFigure();
    void fill(QPolygonF polygon, QColor color);
    FacetType facetType(Facet3D *facet, const QRectF &rect);
    QList<Point3D> framePoints(Facet3D *facet, const QRectF &frame);
    double determinant(Point3D p1, Point3D p2, Point3D p3);

    void renderFigure(const QMatrix4x4 &matrix);
};

#endif // PROJECTION_H
