#include <QtGui>

#include "quadratictree.h"

const int QuadraticTree::POINTS_PER_NODE = 4;

QuadraticTree::QuadraticTree(QRectF rect, const PointsList &points) {
    m_root = new Node(rect, points);
    generateChildren(m_root);
}

void QuadraticTree::generateChildren(Node *node) {
    if (node->points.size() <= POINTS_PER_NODE) return;

    QRectF rect = QRectF(node->rect.topLeft(), node->rect.size() / 2);

    for (int i = 0; i < 4; ++i) {
        int dx = (i == 0 || i == 3 ? 1 : 0);
        int dy = (i == 2 || i == 3 ? 1 : 0);
        node->children.append(new Node(rect.translated(dx * rect.width(), dy * rect.height())));
    }

    foreach (Point point, node->points) {
        foreach (Node *child, node->children) {
            if (child->rect.contains(point)) {
                child->points.append(point);
                break;
            }
        }
    }

    foreach (Node *child, node->children) {
        generateChildren(child);
    }
}

void QuadraticTree::pointsInsideRect(const QRectF &rect, PointsList &pointsList) const {
    pointsList.clear();
    processNode(m_root, rect, pointsList);
}

void QuadraticTree::processNode(Node *node, const QRectF &rect, PointsList &pointsList) const {
    if (!node->rect.intersects(rect)) return;

    if (rect.contains(node->rect)) {
        pointsList.append(node->points);
    } else {
        if (node->children.empty()) {
            foreach (Point point, node->points) {
                if (rect.contains(point)) {
                    pointsList.append(point);
                }
            }
        } else {
            foreach (Node *child, node->children) {
                processNode(child, rect, pointsList);
            }
        }
    }
}
