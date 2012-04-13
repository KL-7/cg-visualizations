#include <QtGui>
#include <QtDebug>

#include "mainwindow.h"
#include "scene.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupScene();
    setupMenu();

    setCentralWidget(m_view);
}

void MainWindow::setupScene() {
    m_scene = new Scene(this);
    m_view = new QGraphicsView(m_scene, this);

    m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_view->setCacheMode(QGraphicsView::CacheBackground);
}

void MainWindow::setupMenu() {
    QMenu *menu = new QMenu(tr("&File"), this);
    menu->addAction(tr("&Inersect"), m_scene, SLOT(findIntersection()), tr("Ctrl+c"));
    menuBar()->addMenu(menu);
}
