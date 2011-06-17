#include "systemtray.h"

#include <QApplication>

void SystemTray::setup()
{
    m_systemTrayIcon = new QSystemTrayIcon(0);

    m_minimizeAction = new QAction(tr("Mi&nimize"), m_systemTrayIcon);
    connect(m_minimizeAction, SIGNAL(triggered()), m_mainWindow,
            SLOT(hide()));

    m_maximizeAction = new QAction(tr("Ma&ximize"), m_systemTrayIcon);
    connect(m_maximizeAction, SIGNAL(triggered()), m_mainWindow,
            SLOT(showMaximized()));

    m_restoreAction = new QAction(tr("&Restore"), m_systemTrayIcon);
    connect(m_restoreAction, SIGNAL(triggered()), m_mainWindow,
            SLOT(showNormal()));

    m_quitAction = new QAction(tr("&Quit"), m_systemTrayIcon);
    connect(m_quitAction, SIGNAL(triggered()), qApp,
            SLOT(quit()));

    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_minimizeAction);
    m_trayIconMenu->addAction(m_maximizeAction);
    m_trayIconMenu->addAction(m_restoreAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);

    m_systemTrayIcon->setContextMenu(m_trayIconMenu);

    m_icon = new QIcon(":/qtmindmap.svg");
    m_systemTrayIcon->setIcon(QIcon(":/qtmindmap.svg"));
}

SystemTray::SystemTray(MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent),
    m_mainWindow(mainWindow)
{
}


void SystemTray::show()
{
    m_systemTrayIcon->show();
}
