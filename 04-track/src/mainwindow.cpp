#include <QtGui>

#include "mainwindow.h"
#include "scene.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupScene();

    setCentralWidget(m_view);
}

void MainWindow::setupScene() {
    m_scene = new Scene(this);
    m_view = new QGraphicsView(m_scene, this);

    m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_view->setCacheMode(QGraphicsView::CacheBackground);
}
