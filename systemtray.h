#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include "mainwindow.h"

class SystemTray : public QSystemTrayIcon
{
//    Q_OBJECT

public:
    SystemTray(MainWindow *parent = 0);

private:
    MainWindow *mainWindow;
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QIcon *icon;
};

#endif // SYSTEMTRAY_H
