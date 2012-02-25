#include <QtGui>

#include "quadratictree.h"

const int QuadraticTree::POINTS_PER_NODE = 4;

QuadraticTree::QuadraticTree(QRectF rect, const QList<QGraphicsEllipseItem *> &points) {
    m_root = new Node(rect, points);
    processNode(m_root);
}

void QuadraticTree::processNode(Node *node) {
    if (node->points.size() <= POINTS_PER_NODE) return;

    QRectF rect = QRectF(node->rect.topLeft(), node->rect.size() / 2);

    for (int i = 0; i < 4; ++i) {
        int dx = (i == 0 || i == 3 ? 1 : 0);
        int dy = (i == 2 || i == 3 ? 1 : 0);
        node->children.append(new Node(rect.translated(dx * rect.width(), dy * rect.height())));
    }

    foreach (QGraphicsEllipseItem *point, node->points) {
        foreach (Node *child, node->children) {
            if (child->rect.contains(point->scenePos())) {
                child->points.append(point);
                break;
            }
        }
    }

    foreach (Node *child, node->children) {
        processNode(child);
    }
}
