#ifndef FACET3D_H
#define FACET3D_H

#include <QtGui>

typedef QVector3D Point3D;
typedef QPair<Point3D, Point3D> Segment3D;

class Facet3D
{
public:
    Facet3D(QList<Point3D> vertices);

    QList<Point3D> vertices() const { return m_vertices; }

    QPolygonF toPolygonF() const;

private:
    QList<Point3D> m_vertices;
};

#endif // FACET3D_H
