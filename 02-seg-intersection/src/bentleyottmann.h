#ifndef BENTLEYOTTMANN_H
#define BENTLEYOTTMANN_H

#include <QtCore>

class BentleyOttmann
{
public:
    BentleyOttmann(const QVector<QLineF> &segments) : m_segments(segments) { }
    QVector<QPointF> intersectionPoints();

private:
    QVector<QLineF> m_segments;
};

#endif // BENTLEYOTTMANN_H
