#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QGraphicsView;
class Scene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow() {}

private:
    Scene *m_scene;
    QGraphicsView *m_view;

    void setupScene();
};

#endif // MAINWINDOW_H
