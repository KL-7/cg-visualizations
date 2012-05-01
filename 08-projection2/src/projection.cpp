#include <QtDebug>
#include "projection.h"

const double Projection::MOVE_SCALE = 10e-2;
const double Projection::ROTATE_SCALE = 10e-1;
const double Projection::ZOOM_SCALE = 10e-2;

Projection::Projection() {
    cop = QVector3D(-5, 0.5, 0.5);  // central of projection

    vrpDistance = 4; // view reference point distance

    vpn = QVector3D(1, 0, 0); // view plane normal
    vup = QVector3D(0, 1, 0); // view-up vector

    viewWindowWidth  = 9;
    viewWindowHeight = 6;

    ff = -1;
    bb = 100;
}

void Projection::render(QGraphicsScene *scene) {
    QMatrix4x4 matrix = transformMatrix()
            * scaleMatrix(scene->width(), -scene->height(), 1)
            * shiftMatrix(0, scene->height(), 0);

    scene->clear();

    renderSegments(scene, unitCube(), matrix);
//    renderSegments(scene, axes(), matrix);
}

void Projection::renderSegments(QGraphicsScene *scene, const QList<Segment3D> &segments, const QMatrix4x4 &matrix) {
    foreach (Segment3D segment, segments) {
        QVector3D p1 = transformPoint3D(segment.first, matrix);
        QVector3D p2 = transformPoint3D(segment.second, matrix);

        scene->addItem(new QGraphicsLineItem(QLineF(p1.toPointF(), p2.toPointF())));
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
