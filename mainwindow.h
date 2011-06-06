#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "aboutdialog.h"
#include "graphwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void klakk();
    void exportScene();
    void about();
    void aboutDestroyed();

private:

    Ui::MainWindow *m_ui;
    AboutDialog *m_aboutDialog;
    GraphWidget *m_graphicsView;

};

#endif // MAINWINDOW_H
