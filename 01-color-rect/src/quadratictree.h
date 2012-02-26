#ifndef QUADRATICTREE_H
#define QUADRATICTREE_H

#include <QtCore>

class QGraphicsEllipseItem;

typedef QPointF Point;
typedef QList<Point> PointsList;

struct Node {
    QRectF rect;
    PointsList points;
    QList<struct Node*> children;

    Node(QRectF rect, const PointsList &points = PointsList()) : rect(rect) {
        this->points.append(points);
    }

    ~Node() {
        foreach(struct Node* child, children) {
            delete child;
        }
    }
} typedef Node;

class QuadraticTree
{
public:
    QuadraticTree(QRectF rect, const PointsList &points);
    ~QuadraticTree() { delete m_root; }

    void pointsInsideRect(const QRectF &rect, PointsList &pointsList) const;
    Node* root() { return m_root; }

private:
    Node *m_root;

    static const int POINTS_PER_NODE;

    void generateChildren(Node *node);
    void processNode(Node *node, const QRectF &rect, PointsList &itemsList) const;
};

#endif // QUADRATICTREE_H
