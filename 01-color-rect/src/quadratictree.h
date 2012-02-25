#ifndef QUADRATICTREE_H
#define QUADRATICTREE_H

#include <QtCore>

class QGraphicsEllipseItem;

struct Node {
    QRectF rect;
    QList<QGraphicsEllipseItem*> points;
    QList<struct Node*> children;

    Node(QRectF rect, const QList<QGraphicsEllipseItem*> &points = QList<QGraphicsEllipseItem*>()) : rect(rect) {
        this->points.append(points);
    }
} typedef Node;

class QuadraticTree
{
public:
    QuadraticTree(QRectF rect, const QList<QGraphicsEllipseItem*> &points);
    const QList<QGraphicsEllipseItem*>& pointsInRect(QRect rect) const;
    Node* root() { return m_root; }

private:
    Node *m_root;

    static const int POINTS_PER_NODE;

    void processNode(Node *node);
};

#endif // QUADRATICTREE_H
