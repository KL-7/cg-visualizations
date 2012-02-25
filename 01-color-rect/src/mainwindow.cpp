#include <QtGui>
#include <QtDebug>

#include "mainwindow.h"
#include "scene.h"
#include "regeneratedialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
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
    menuBar()->addAction(tr("&Regenerate"), this, SLOT(regenerate()));
}

void MainWindow::regenerate() {
    RegenerateDialog dialog(m_scene->pointsCount(), m_scene->rectSize(), this);
    if (dialog.exec()) {
        m_scene->regenerate(dialog.pointsCount(), dialog.rectSize());
    }
}
