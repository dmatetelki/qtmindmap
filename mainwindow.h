#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QPointer>

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
    void klakk(void);
    void about();
    void aboutDestroyed();

private:
    Ui::MainWindow *ui;
    AboutDialog* aboutDialog;
//    QPointer<AboutDialog> aboutDialog;
    GraphWidget *graphicsView;
};

#endif // MAINWINDOW_H
