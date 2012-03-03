#include <QtGui>
#include <QtDebug>

#include "mainwindow.h"
#include "scene.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    qsrand(QTime::currentTime().msec());

    setupScene();
    setupMenu();

    setCentralWidget(m_view);
}

void MainWindow::setupScene() {
    m_scene = new Scene(this);
    m_view = new QGraphicsView(m_scene, this);

//    m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_view->setCacheMode(QGraphicsView::CacheBackground);
}

void MainWindow::setupMenu() {
    QMenu *menu = new QMenu(tr("File"), this);
    menu->addAction(tr("&Color"), m_scene, SLOT(colorIntersections()));
    menu->addAction(tr("&Fast Color"), m_scene, SLOT(fastColorIntersections()));
    menuBar()->addMenu(menu);
}