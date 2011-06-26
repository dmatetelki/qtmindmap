#include "include/systemtray.h"

#include <QApplication>


SystemTray::SystemTray(MainWindow *mainWindow, QWidget *parent)
    : QWidget(parent),
      m_parent(mainWindow)
{
    m_systemTrayIcon = new QSystemTrayIcon(0);

    m_minimizeAction = new QAction(tr("Mi&nimize"), m_systemTrayIcon);
    connect(m_minimizeAction, SIGNAL(triggered()), m_parent,
            SLOT(hide()));

    m_maximizeAction = new QAction(tr("Ma&ximize"), m_systemTrayIcon);
    connect(m_maximizeAction, SIGNAL(triggered()), m_parent,
            SLOT(showMaximized()));

    m_restoreAction = new QAction(tr("&Restore"), m_systemTrayIcon);
    connect(m_restoreAction, SIGNAL(triggered()), m_parent,
            SLOT(showNormal()));

    m_quitAction = new QAction(tr("&Quit"), m_systemTrayIcon);
    connect(m_quitAction, SIGNAL(triggered()), m_parent,
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

void SystemTray::show()
{
    m_systemTrayIcon->show();
}
