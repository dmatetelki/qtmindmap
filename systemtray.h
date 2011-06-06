#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include "mainwindow.h"

class SystemTray : public QWidget
{
    Q_OBJECT

public:

    explicit SystemTray(MainWindow *mainWindow, QWidget *parent = 0);
    void setup();
    void show();

private:

        MainWindow *m_mainWindow;

        QSystemTrayIcon *m_systemTrayIcon;
        QMenu *m_trayIconMenu;
        QAction *m_minimizeAction;
        QAction *m_maximizeAction;
        QAction *m_restoreAction;
        QAction *m_quitAction;
        QIcon *m_icon;

};

#endif // SYSTEMTRAY_H
