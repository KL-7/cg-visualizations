#ifndef POINT3D_H
#define POINT3D_H

class Point3D
{
public:
    Point3D(double x, double y, double z);

    double x() const { return m_x; }
    double y() const { return m_y; }
    double z() const { return m_z; }

private:
    double m_x;
    double m_y;
    double m_z;
};



#endif // POINT3D_H
