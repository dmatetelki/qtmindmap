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

    Ui::MainWindow *m_ui;
    AboutDialog *m_aboutDialog;
    GraphWidget *m_graphicsView;

    QSystemTrayIcon *m_systemTrayIcon;
    MainWindow *m_mainWindow;
    QMenu *m_trayIconMenu;
    QAction *m_minimizeAction;
    QAction *m_maximizeAction;
    QAction *m_restoreAction;
    QAction *m_quitAction;
    QIcon *m_icon;
};

#endif // MAINWINDOW_H
