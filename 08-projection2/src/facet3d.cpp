#include "facet3d.h"

Facet3D::Facet3D(QList<Point3D> vertices) : m_vertices(vertices) { }

QPolygonF Facet3D::toPolygonF() const {
    QVector<QPointF> points;

    foreach (Point3D vertex, m_vertices) {
        points << vertex.toPointF();
    }

    return QPolygonF(points);
}
