#include "systemtray.h"


#include <QtGui>
//#include <QAction>
//#include <QMenu>

SystemTray::SystemTray(MainWindow* parent)
    : QSystemTrayIcon(parent),
    mainWindow(parent)
{
    //        QSystemTrayIcon *trayIcon;


    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), parent, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), parent, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), parent, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    /// @BUG shall pass this as parent
    trayIconMenu = new QMenu(0);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

//    trayIcon = new QSystemTrayIcon(this);
//    trayIcon->setContextMenu(trayIconMenu);
    setContextMenu(trayIconMenu);

    icon = new QIcon(":/heart.svg");
    setIcon(QIcon(":/heart.svg"));
}
