#include <QtGui>
#include <QDebug>

#include "scene.h"
#include "rect.h"
#include "quadratictree.h"

const QRect Scene::DEFAULT_SCENE_RECT = QRect(0, 0, 900, 600);
const QRect Scene::RECT_GEOMETRY = QRect(0, 0, 220, 180);
const double Scene::POINT_RADIUS = 1;
const int Scene::POINTS_COUNT = 100000;
const QColor Scene::INNER_POINT_COLOR = Qt::yellow;
const QColor Scene::OUTER_POINT_COLOR = Qt::black;

Scene::Scene(QObject *parent) : QGraphicsScene(parent), m_fast(false) {
    setBackgroundBrush(QColor("#DAEDDC"));
    setSceneRect(DEFAULT_SCENE_RECT);

    addRect();
    addPoints();
}

void Scene::addPoints(int count) {
    for (int i = 0; i < count; ++i) {
        m_points.append(randomPoint());
    }

    m_tree = new QuadraticTree(sceneRect(), m_points);

//    drawQuadraticTree();
    recolorPoints();
}

void Scene::drawQuadraticTree() {
    QQueue<Node*> nodes;
    nodes.enqueue(m_tree->root());

    while (!nodes.empty()) {
        Node *node = nodes.dequeue();
        QGraphicsRectItem *rect = new QGraphicsRectItem(node->rect);
        rect->setPen(QPen(Qt::DotLine));
        addItem(rect);
        foreach(Node *child, node->children) {
            nodes.enqueue(child);
        }
    }
}

QGraphicsEllipseItem* Scene::randomPoint()
{
    QGraphicsEllipseItem *point = new QGraphicsEllipseItem(0, 0, POINT_RADIUS, POINT_RADIUS, 0, this);
    point->setPos(rand(width()), rand(height()));
    point->setPen(QPen(OUTER_POINT_COLOR, POINT_RADIUS));
    return point;
}

double Scene::rand(double max)
{
    return max * qrand() / RAND_MAX;
}

QSize Scene::rectSize() const {
    return m_rect->rect().size().toSize();
}

int Scene::pointsCount() const {
    return m_points.size();
}

void Scene::addRect()
{
    m_rect = new Rect(RECT_GEOMETRY, 0, this);
    m_rect->setZValue(-1);
}

void Scene::removePoints() {
    foreach (QGraphicsItem *point, m_points) {
        removeItem(point);
    }

    m_points.clear();
}

void Scene::regenerate(int pointsCount, QSize rectSize) {
    setRectSize(rectSize);
    regeneratePoints(pointsCount);
}

void Scene::regeneratePoints(int count) {
    removePoints();
    addPoints(count);
}

void Scene::setRectSize(QSize rectSize) {
    m_rect->setRect(QRect(QPoint(0, 0), rectSize));
}

void Scene::switchMode(bool fast) {
    m_fast = fast;
    if (m_fast) {
        m_tree->pointsInsideRect(m_rect->sceneBoundingRect(), m_pointsInsideRect);
    }
}

void Scene::recolorPoints() {
    if (m_fast) {
        foreach (QGraphicsEllipseItem *point, m_pointsInsideRect) {
            point->setPen(QPen(OUTER_POINT_COLOR, POINT_RADIUS));
        }

        m_tree->pointsInsideRect(m_rect->sceneBoundingRect(), m_pointsInsideRect);

        foreach (QGraphicsEllipseItem *point, m_pointsInsideRect) {
            point->setPen(QPen(INNER_POINT_COLOR, POINT_RADIUS));
        }
    } else {
        foreach (QGraphicsEllipseItem *point, m_points) {
            QColor color = m_rect->rect().contains(m_rect->mapFromScene(point->scenePos())) ? INNER_POINT_COLOR : OUTER_POINT_COLOR;
            if (point->pen().color() != color) {
                point->setPen(QPen(color, POINT_RADIUS));
            }
        }
    }
}
