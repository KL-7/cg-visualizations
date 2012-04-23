#include <QtDebug>
#include "projection.h"

Projection::Projection() {
    vrp = QVector3D(-1, 0, 0); // view reference point
    cop = QVector3D(-3, 1, 1); // central of projection

    vpn = QVector3D(1, 0, 0); // view plane normal
    vup = QVector3D(0, 1, 0); // view-up vector

    uvMin = QPointF(0, 0);
    uvMax = QPointF(2, 2);

    ff = -1;
    bb = 4;
}

QPixmap Projection::render(QSize size) {
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QMatrix4x4 matrix = transformMatrix();

    return pixmap;
}

QMatrix4x4 Projection::transformMatrix() {
    qDebug() << "transformMatrix START";

    QMatrix4x4 matrix;

    // matrx = T(-COP)
    matrix *= QMatrix4x4(
               1,        0,         0, 0,
               0,        1,         0, 0,
               0,        0,         1, 0,
        -cop.x(), -cop.y(),  -cop.z(), 1
    );

    qDebug() << "* T(-COP): " << matrix;

    QVector3D rz = -vpn.normalized();
    QVector3D rx = QVector3D::crossProduct(vpn, vup).normalized();
    QVector3D ry = QVector3D::crossProduct(rz, rx); // (rz x rx), NOT (rx x rz)!

    QMatrix4x4 r;
    r.setColumn(0, rx);
    r.setColumn(1, ry);
    r.setColumn(2, rz);

    qDebug() << "r: " << r;

    // matrix = T(-COP) * R
    matrix *= r;

    qDebug() << "* T(-COP) * R: " << matrix;

    // matrx = T(-COP) * R * T_pl
    matrix *= QMatrix4x4(
        1, 0,  0, 0,
        0, 1,  0, 0,
        0, 0, -1, 0,
        0, 0,  0, 1
    );

    qDebug() << "* T(-COP) * R * T_pl: " << matrix;

    // vrp1 = vrp * T(-COP) * R * T_pl
    QVector3D vrp1 = transformPoint(vrp, matrix);

    double wcx = vrp1.x() + (uvMin.x() + uvMax.x()) / 2;
    double wcy = vrp1.y() + (uvMin.y() + uvMax.y()) / 2;
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

    qDebug() << "* T(-COP) * R * T_pl * Sh(a, b): " << matrix;

    double sz = 1.0 / (vrp1.z() + bb);
    double sx = 2 * vrp1.z() * sz / (uvMax.x() - uvMin.x());
    double sy = 2 * vrp1.z() * sz / (uvMax.y() - uvMin.y());

    // matrx = T(-COP) * R * T_pl * Sh(a, b) * S_c
    matrix *= QMatrix4x4(
        sx,  0,  0, 0,
         0, sy,  0, 0,
         0,  0, sz, 0,
         0,  0,  0, 1
    );

    qDebug() << "* T(-COP) * R * T_pl * Sh(a, b) * S_c: " << matrix;

    qDebug() << "transformMatrix END";

    return matrix;
}

QVector3D Projection::transformPoint(QVector3D p, QMatrix4x4 m) {
    return (QVector4D(p.x(), p.y(), p.z(), 1) * m).toVector3DAffine();
}
