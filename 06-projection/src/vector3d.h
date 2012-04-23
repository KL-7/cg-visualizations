#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "point3d.h"

class Vector3D
{
public:
    Vector3D(Point3D p1, Point3D p2);

    Point3D p1() const { return m_p1; }
    Point3D p2() const { return m_p2; }

private:
    Point3D m_p1;
    Point3D m_p2;
};

#endif // VECTOR3D_H
