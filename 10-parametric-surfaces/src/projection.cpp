#include <QtDebug>
#include "projection.h"

const double Projection::MOVE_SCALE = 10e-2;
const double Projection::ROTATE_SCALE = 10e-1;
const double Projection::ZOOM_SCALE = 10e-2;
const double Projection::POINT_RAIDUS = 0.5;
const int Projection::GRID_SIZE = 50;

// Axes directions: x - forward, y - up, z - right.

Projection::Projection() {
    cop = QVector3D(-1, 0.5, 0.5);  // center of projection

    vrpDistance = 4; // view reference point distance

    vpn = QVector3D(1, 0, 0); // view plane normal
    vup = QVector3D(0, 1, 0); // view-up vector

    viewWindowWidth  = 9;
    viewWindowHeight = 6;

    ff = -1;
    bb = 10;

    if (pS0(0) != p0T(0)) qDebug() << "p(s, 0) != p(0, t) in (0, 0)";
    if (pS0(1) != p1T(0)) qDebug() << "p(s, 0) != p(1, t) in (1, 0)";
    if (pS1(0) != p0T(1)) qDebug() << "p(s, 1) != p(0, t) in (0, 1)";
    if (pS1(1) != p1T(1)) qDebug() << "p(s, 1) != p(1, t) in (1, 1)";

    m_surfacePoints = surfacePoints();
}

void Projection::render(QGraphicsScene *scene) {
    QMatrix4x4 matrix = transformMatrix()
            * scaleMatrix(scene->width(), -scene->height(), 1)
            * shiftMatrix(0, scene->height(), 0);

    scene->clear();

    renderPoints(scene, m_surfacePoints, matrix);

//    renderSegments(scene, unitCube(), matrix);
//    renderSegments(scene, axes(), matrix);
}

Point3D Projection::pS0(double s) {
    return Point3D(s, s - s * s, 0);
}

Point3D Projection::pS1(double s) {
    return Point3D(s, s - s * s, 1);
}

Point3D Projection::p0T(double t) {
    return Point3D(0, t - t * t, t);
}

Point3D Projection::p1T(double t) {
    return Point3D(1, t - t * t, t);
}

Point3D Projection::cST(double s, double t) {
    return qST(s, t) - bST(s, t);
}

Point3D Projection::qST(double s, double t) {
    return t * pS1(s) + (1 - t) * pS0(s) + s * p1T(t) + (1 - s) * p0T(t);
}

Point3D Projection::bST(double s, double t) {
    return s * (t * p1T(1) + (1 - t) * p1T(0)) + (1 - s) * (t * p0T(1) + (1 - t) * p0T(0));
}

QList<Point3D> Projection::surfacePoints() {
    double step = 1.0 / GRID_SIZE;
    QList<Point3D> points;

    for (double s = 0; s <= 1; s += step) {
        for (double t = 0; t <= 1; t += step) {
            points << cST(s, t);
        }
    }

    return points;
}

void Projection::renderPoints(QGraphicsScene *scene, const QList<Point3D> &points, const QMatrix4x4 &matrix) {
    foreach (Point3D point, points) {
        QRectF rect = QRectF(transformPoint(point, matrix), QSizeF(POINT_RAIDUS, POINT_RAIDUS))
                .translated(-POINT_RAIDUS, -POINT_RAIDUS);
        scene->addItem(new QGraphicsEllipseItem(rect));
    }
}

void Projection::renderSegments(QGraphicsScene *scene, const QList<Segment3D> &segments, const QMatrix4x4 &matrix) {
    foreach (Segment3D segment, segments) {
        QLineF line(transformPoint(segment.first, matrix), transformPoint(segment.second, matrix));
        scene->addItem(new QGraphicsLineItem(line));
    }
}

QMatrix4x4 Projection::transformMatrix() {
    QMatrix4x4 matrix;

    // matrx = T(-COP)
    matrix *= shiftMatrix(-cop);

    QVector3D rz = -vpn.normalized();
    QVector3D rx = QVector3D::crossProduct(vpn, vup).normalized();
    QVector3D ry = QVector3D::crossProduct(rz, rx); // (rz x rx), NOT (rx x rz)!

    QMatrix4x4 r;
    r.setColumn(0, rx);
    r.setColumn(1, ry);
    r.setColumn(2, rz);

    // matrix = T(-COP) * R
    matrix *= r;

    // matrx = T(-COP) * R * T_pl
    matrix *= scaleMatrix(1, 1, -1);

    // vrp1 = vrp * T(-COP) * R * T_pl
    QVector3D vrp1 = transformPoint3D(vrp(), matrix);

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

QVector3D Projection::transformPoint3D(QVector3D p, QMatrix4x4 m) {
    return (QVector4D(p.x(), p.y(), p.z(), 1) * m).toVector3DAffine();
}

QPointF Projection::transformPoint(QVector3D p, QMatrix4x4 m) {
    return transformPoint3D(p, m).toPointF();
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

QMatrix4x4 Projection::shiftMatrix(QVector3D v) {
    return shiftMatrix(v.x(), v.y(), v.z());
}

QList<Segment3D> Projection::unitCube() {
    QList<Segment3D> segments;

    segments
        << segment3D(0, 0, 0, 1, 0, 0)
        << segment3D(1, 0, 0, 1, 1, 0)
        << segment3D(1, 1, 0, 0, 1, 0)
        << segment3D(0, 1, 0, 0, 0, 0)

        << segment3D(0, 0, 1, 1, 0, 1)
        << segment3D(1, 0, 1, 1, 1, 1)
        << segment3D(1, 1, 1, 0, 1, 1)
        << segment3D(0, 1, 1, 0, 0, 1)

        << segment3D(0, 0, 0, 0, 0, 1)
        << segment3D(1, 0, 0, 1, 0, 1)
        << segment3D(1, 1, 0, 1, 1, 1)
        << segment3D(0, 1, 0, 0, 1, 1)
    ;


    return segments;
}

QList<Segment3D> Projection::axes() {
    QList<Segment3D> segments;

    segments
        << segment3D(0, 0, 0, 100,   0,   0)
        << segment3D(0, 0, 0,   0, 100,   0)
        << segment3D(0, 0, 0,   0,   0, 100)
    ;

    return segments;
}

Segment3D Projection::segment3D(double x1, double y1, double z1, double x2, double y2, double z2) {
    return Segment3D(QVector3D(x1, y1, z1), QVector3D(x2, y2, z2));
}

void Projection::move(double horizontal, double vertical) {
    cop = transformPoint3D(cop, shiftMatrix(MOVE_SCALE * (horizontal * QVector3D::crossProduct(vpn, vup) + vertical * vpn)));
}

void Projection::rotate(double horizontal, double vertical) {
    QVector3D horizontalAxis = QVector3D::crossProduct(vpn, vup);
    QVector3D verticalAxis = QVector3D::crossProduct(horizontalAxis, vpn);

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
