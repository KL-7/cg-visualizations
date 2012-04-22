#include <QtGui>

#include "mainwindow.h"
#include "scene.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupScene();
    setupMenu();
}

void MainWindow::setupScene() {
    m_scene = new Scene(this);
    m_view = new QGraphicsView(m_scene, this);

    m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_view->setCacheMode(QGraphicsView::CacheBackground);

    setCentralWidget(m_view);
}

void MainWindow::setupMenu() {
    QMenu *menu = new QMenu(tr("&File"), this);
    menu->addAction(tr("&Generate points"), m_scene, SLOT(generatePoints()), tr("Ctrl+G"));
    menu->addAction(tr("Convex H&ull"), m_scene, SLOT(buildConvexHull()), tr("Ctrl+U"));
    menuBar()->addMenu(menu);
}
