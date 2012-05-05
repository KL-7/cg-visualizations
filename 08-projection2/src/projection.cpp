#include <QtDebug>

#include "projection.h"
#include "intersection.h"

const double Projection::MOVE_SCALE = 10e-2;
const double Projection::ROTATE_SCALE = 10e-1;
const double Projection::ZOOM_SCALE = 10e-2;
const double Projection::PIXELIZATION_LIMIT = 1;

Projection::Projection(QGraphicsScene *scene) : m_scene(scene) {
    cop = Point3D(-3, 0.5, 0.5);  // center of projection

    vrpDistance = 4; // view reference point distance

    vpn = Point3D(1, 0, 0); // view plane normal
    vup = Point3D(0, 1, 0); // view-up vector

    viewWindowWidth  = 9;
    viewWindowHeight = 6;

    ff = -1;
    bb = 100;

    setupFigure();
}

void Projection::render() {
    QMatrix4x4 matrix = transformMatrix()
            * scaleMatrix(m_scene->width(), -m_scene->height(), 1)
            * shiftMatrix(0, m_scene->height(), 0);

    m_scene->clear();
    renderFigure(matrix);
}

void Projection::renderFigure(const QMatrix4x4 &matrix) {
    foreach (Facet3D *facet, m_figure) {
        m_scene->addPolygon(transformFacet3D(facet, matrix)->toPolygonF());
    }

    QList<Facet3D*> transformedFigure;

    foreach (Facet3D *facet, m_figure) {
        transformedFigure << transformFacet3D(facet, matrix);
    }

    warnock(m_scene->sceneRect(), QList<Facet3D*>(), transformedFigure);
}

void Projection::warnock(const QRectF &rect, QList<Facet3D*> enclosingFacets, QList<Facet3D*> otherFacets) {
    // TODO: handle single pixel.
    if (rect.width() * rect.height() <= PIXELIZATION_LIMIT) return;

    QList<Facet3D*> intersectingFacets, innerFacets;

    foreach (Facet3D *facet, otherFacets) {
        switch (facetType(facet, rect)) {
        case ENCLOSING:
            enclosingFacets << facet;
            break;
        case INTERSECTING:
            intersectingFacets << facet;
            break;
        case INNER:
            innerFacets << facet;
            break;
        }
    }

    // 1. No intersection - background color.
    if (enclosingFacets.empty() && intersectingFacets.empty() && innerFacets.empty()) return;

    // 2. Single inner or intersecting facet.
    if (enclosingFacets.empty() && (intersectingFacets.size() + innerFacets.size() == 1)) {
        if (intersectingFacets.size() == 1) {
            Facet3D *facet = intersectingFacets.first();
            fill(Intersection::calculate(QPolygonF(rect), facet->toPolygonF()), facet->color());
        } else /*if (innerFacets.size() == 1)*/ {
            Facet3D *facet = innerFacets.first();
            fill(facet->toPolygonF(), facet->color());
        }
        return;
    }

    // 3. Single enclosing facet.
    if (enclosingFacets.size() == 1 && intersectingFacets.empty() && innerFacets.empty()) {
        fill(QPolygonF(rect), enclosingFacets.first()->color());
        return;
    }

    // 4. At least one enclosing facet and several inner and intersecting facets.
    if (enclosingFacets.size() >= 1/* && (intersectingFacets.size() + innerFacets.size() > 1)*/) {
        QList<Point3D> bestPoints;
        Facet3D *bestFacet = 0;

        foreach (Facet3D *facet, enclosingFacets + intersectingFacets + innerFacets) {
            QList<Point3D> points = framePoints(facet, rect);

            bool better = true;

            if (!bestPoints.empty()) {
                for (int i = 0; i < 4; ++i) {
                    if (points[i].z() > bestPoints[i].z()) {
                        better = false;
                        break;
                    }
                }
            }

            if (better) {
                bestPoints = points;
                bestFacet = facet;
            }

            foreach (Facet3D *facet, intersectingFacets + innerFacets) {
                if (facet == bestFacet) {
                    bestFacet = 0;
                    break;
                }
            }
        }

        if (bestFacet) {
            fill(Intersection::calculate(QPolygonF(rect), bestFacet->toPolygonF()), bestFacet->color());
            return;
        }
    }

    QRectF r(rect.left(), rect.top(), rect.width() / 2, rect.height() / 2);
    QList<Facet3D*> facets = intersectingFacets + innerFacets;

    for (int i = 0; i < 4; ++i) {
        warnock(r.translated((i % 2) * r.width(), (i / 2) * r.height()), enclosingFacets, facets);
    }
}

QList<Point3D> Projection::framePoints(Facet3D *facet, const QRectF &frame) {
    Point3D p1 = facet->vertices()[0],
            p2 = facet->vertices()[1],
            p3 = facet->vertices()[2];

    double D = p1.x() * (p2.y() * p3.z() - p2.z() * p3.y())
             - p1.y() * (p2.x() * p3.z() - p2.z() * p3.x())
             + p1.z() * (p2.x() * p3.y() - p2.y() * p3.x());

    double A =      1 * (p2.y() * p3.z() - p2.z() * p3.y())
             - p1.y() * (     1 * p3.z() - p2.z() *      1)
             + p1.z() * (     1 * p3.y() - p2.y() *      1);

    double B = p1.x() * (     1 * p3.z() - p2.z() *      1)
             -      1 * (p2.x() * p3.z() - p2.z() * p3.x())
             + p1.z() * (p2.x() *      1 -      1 * p3.x());

    double C = p1.x() * (p2.y() *      1 -      1 * p3.y())
             - p1.y() * (p2.x() *      1 -      1 * p3.x())
             +      1 * (p2.x() * p3.y() - p2.y() * p3.x());

    double a = -A/D, b = -B/D, c = -C/D;

    QList<Point3D> points;

    foreach (QPointF p, QPolygonF(frame).toList()) {
        points << Point3D(p.x(), p.y(), (a * p.x() + b * p.y() + 1) / -c);
    }

    return points;
}

double Projection::determinant(Point3D p1, Point3D p2, Point3D p3) {
    return p1.x() * (p2.y() * p3.z() - p2.z() * p3.y())
         - p1.y() * (p2.x() * p3.z() - p2.z() * p3.x())
         + p1.z() * (p2.x() * p3.y() - p2.y() * p3.x());
}

Projection::FacetType Projection::facetType(Facet3D *facet, const QRectF &rect) {
    QPolygonF polygon = facet->toPolygonF();

    bool inside = true;
    foreach (QPointF p, QPolygonF(rect).toList()) {
        if (!polygon.containsPoint(p, Qt::OddEvenFill)) {
            inside = false;
            break;
        }
    }
    if (inside) return ENCLOSING;

    QPolygonF intersection = Intersection::calculate(rect, polygon);

    if (intersection.empty()) {
        return OUTER;
    } else if (intersection == polygon) {
        return INNER;
    }

    return INTERSECTING;
}

void Projection::fill(QPolygonF polygon, QColor color) {
    m_scene->addPolygon(polygon, color, color);
}


QMatrix4x4 Projection::transformMatrix() {
    QMatrix4x4 matrix;

    // matrx = T(-COP)
    matrix *= shiftMatrix(-cop);

    Point3D rz = -vpn.normalized();
    Point3D rx = Point3D::crossProduct(vpn, vup).normalized();
    Point3D ry = Point3D::crossProduct(rz, rx); // (rz x rx), NOT (rx x rz)!

    QMatrix4x4 r;
    r.setColumn(0, rx);
    r.setColumn(1, ry);
    r.setColumn(2, rz);

    // matrix = T(-COP) * R
    matrix *= r;

    // matrx = T(-COP) * R * T_pl
    matrix *= scaleMatrix(1, 1, -1);

    // vrp1 = vrp * T(-COP) * R * T_pl
    Point3D vrp1 = transformPoint3D(vrp(), matrix);

    double wcx = vrp1.x() + (uvMin().x() + uvMax().x()) / 2;
    double wcy = vrp1.y() + (uvMin().y() + uvMax().y()) / 2;
    double wcz = vrp1.z();

    double a = -wcx / wcz;
    double b = -wcy / wcz;

    // matrx = T(-COP) * R * T_pl * Sh(a, b)
    matrix *= QMatrix4x4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        a, b, 1, 0, // a, b, 1(!), 0
        0, 0, 0, 1
    );

    double sz = 1.0 / (vrp1.z() + bb);
    double sx = 2 * vrp1.z() * sz / (uvMax().x() - uvMin().x());
    double sy = 2 * vrp1.z() * sz / (uvMax().y() - uvMin().y());

    // matrx = T(-COP) * R * T_pl * Sh(a, b) * S_c
    matrix *= scaleMatrix(sx, sy, sz);

    double zMin = (vrp1.z() + ff) / (vrp1.z() + bb);

    // mamtrix = T(-COP) * R * T_pl * Sh(a, b) * S_c * M
    matrix *= QMatrix4x4(
        1, 0,                  0, 0,
        0, 1,                  0, 0,
        0, 0,     1 / (1 - zMin), 1,
        0, 0, -zMin / (1 - zMin), 0
    );

    // mamtrix = T(-COP) * R * T_pl * Sh(a, b) * S_c * M * T(1, 1, 0)
    matrix *= shiftMatrix(1, 1, 0);

    // mamtrix = T(-COP) * R * T_pl * Sh(a, b) * S_c * M * T(1, 1, 0) * S(1/2, 1/2, 1)
    matrix *= scaleMatrix(0.5, 0.5, 1);

    return matrix;
}

Point3D Projection::transformPoint3D(Point3D p, QMatrix4x4 m) {
    return (QVector4D(p.x(), p.y(), p.z(), 1) * m).toVector3DAffine();
}

QPointF Projection::transformPoint(Point3D p, QMatrix4x4 m) {
    return transformPoint3D(p, m).toPointF();
}

Facet3D* Projection::transformFacet3D(Facet3D *facet, QMatrix4x4 m) {
    QList<Point3D> points;

    foreach (Point3D vertex, facet->vertices()) {
        points << transformPoint3D(vertex, m);
    }

    return new Facet3D(points, facet->id(), facet->color());
}

QMatrix4x4 Projection::shiftMatrix(double dx, double dy, double dz) {
    return QMatrix4x4(
         1,  0,  0, 0,
         0,  1,  0, 0,
         0,  0,  1, 0,
        dx, dy, dz, 1
    );
}

QMatrix4x4 Projection::scaleMatrix(double sx, double sy, double sz) {
    return QMatrix4x4(
        sx,  0,  0, 0,
         0, sy,  0, 0,
         0,  0, sz, 0,
         0,  0,  0, 1
    );
}

QMatrix4x4 Projection::shiftMatrix(Point3D v) {
    return shiftMatrix(v.x(), v.y(), v.z());
}

void Projection::setupFigure() {
    QList<Point3D> points;

    points
        << Point3D(0, 0, 0)
        << Point3D(1, 0, 0)
        << Point3D(1, 0, 1)
        << Point3D(0, 0, 1)
    ;
    m_figure << new Facet3D(points, 1, Qt::darkGray);

    points.clear();
    points
        << Point3D(0,   0, 0)
        << Point3D(1,   0, 0)
        << Point3D(0.5, 1, 0.5)
    ;
    m_figure << new Facet3D(points, 2, Qt::red);

    points.clear();
    points
        << Point3D(1,   0, 0)
        << Point3D(1,   0, 1)
        << Point3D(0.5, 1, 0.5)
    ;
    m_figure << new Facet3D(points, 3, Qt::blue);

    points.clear();
    points
        << Point3D(1,   0, 1)
        << Point3D(0,   0, 1)
        << Point3D(0.5, 1, 0.5)
    ;
    m_figure << new Facet3D(points, 4, Qt::green);

    points.clear();
    points
        << Point3D(0,   0, 1)
        << Point3D(0,   0, 0)
        << Point3D(0.5, 1, 0.5)
    ;
    m_figure << new Facet3D(points, 5, Qt::yellow);
}

void Projection::move(double horizontal, double vertical) {
    cop = transformPoint3D(cop, shiftMatrix(MOVE_SCALE * (horizontal * Point3D::crossProduct(vpn, vup) + vertical * vpn)));
}

void Projection::rotate(double horizontal, double vertical) {
    Point3D horizontalAxis = Point3D::crossProduct(vpn, vup);
    Point3D verticalAxis = Point3D::crossProduct(horizontalAxis, vpn);

    QMatrix4x4 horizontalRotation;
    horizontalRotation.rotate(ROTATE_SCALE * horizontal, verticalAxis);

    QMatrix4x4 verticalRotation;
    verticalRotation.rotate(-ROTATE_SCALE * vertical, horizontalAxis);

    vup = transformPoint3D(transformPoint3D(vup, horizontalRotation), verticalRotation);
    vpn = transformPoint3D(transformPoint3D(vpn, horizontalRotation), verticalRotation);
}

void Projection::zoom(double factor) {
    double dist = factor * ZOOM_SCALE;

    vrpDistance -= dist;
    cop += dist * vpn.normalized();
}
