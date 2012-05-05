#ifndef FACET3D_H
#define FACET3D_H

#include <QtGui>

typedef QVector3D Point3D;
typedef QPair<Point3D, Point3D> Segment3D;

class Facet3D
{
public:
    Facet3D(QList<Point3D> vertices, int id, QColor color = Qt::white);

    QList<Point3D> vertices() const { return m_vertices; }
    QColor color() const { return m_color; }
    int id() const { return m_id; }

    QPolygonF toPolygonF() const;

private:
    QList<Point3D> m_vertices;
    QColor m_color;
    int m_id;
};

QDebug operator<<(QDebug debug, const Facet3D &facet);
QDebug operator<<(QDebug debug, const QList<Facet3D*> &facets);

#endif // FACET3D_H
