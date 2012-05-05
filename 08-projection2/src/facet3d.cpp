#include "facet3d.h"

Facet3D::Facet3D(QList<Point3D> vertices, int id, QColor color) : m_vertices(vertices), m_color(color), m_id(id) { }

QPolygonF Facet3D::toPolygonF() const {
    QVector<QPointF> points;

    foreach (Point3D vertex, m_vertices) {
        points << vertex.toPointF();
    }

    return QPolygonF(points);
}

QDebug operator<<(QDebug debug, const Facet3D &facet) {
    debug.nospace() << "Facet3D(" << facet.id() << ", " << facet.vertices() << ", " << facet.color() << ")";
    return debug.maybeSpace();
}

QDebug operator<<(QDebug debug, const QList<Facet3D*> &facets) {
    debug << "QList<Facet3D*>(";

    foreach (Facet3D* facet, facets) {
        debug << *facet << ",";
    }

    debug << ")";
    return debug.maybeSpace();
}
