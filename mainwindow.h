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
    explicit MainWindow(bool isSystemTray = false, QWidget *parent = 0);
    ~MainWindow();
    void showSysTray();

public slots:
    void klakk();
    void exportScene();
    void about();
    void aboutDestroyed();

private:

    void setupSystemTray();

    Ui::MainWindow *ui;
    AboutDialog* aboutDialog;
    GraphWidget *graphicsView;

    QSystemTrayIcon *systemTrayIcon;
    MainWindow *mainWindow;
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QIcon *icon;
};

#endif // MAINWINDOW_H
