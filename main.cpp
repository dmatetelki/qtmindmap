#include <stdlib.h> // EXIT_SUCCESS
#include <iostream> // cout

#include <QtGui>
//#include <QtGui/QApplication>
#include <QDebug>
#include <QRegExp>

#include "mainwindow.h"
#include "systemtray.h"


void printUsage()
{
    std::cout << "Usage: qtmindmap [OPTION][FILE]" << std::endl
              << "Mindmap program in QT" << std::endl
              << std::endl
              << "Options:" << std::endl
              << "-h,  --help\tPrints this help." << std::endl
              << "-t,  --tray\tStarts application in system tray." << std::endl
              << std::endl
              << "Report bugs to: denes.matetelki@gmail.com" << std::endl;
}


bool parseCmdLineArgs(bool &isSystemTray, QString &filePath)
{
    QStringList cmdlineArgs = QCoreApplication::arguments();
    cmdlineArgs.removeFirst();

    QRegExp help("^-(h|-help)$");
    if (!cmdlineArgs.filter(help).isEmpty()) return false;

    QRegExp tray("^-(t|-tray)$");
    if (!cmdlineArgs.filter(tray).isEmpty()) isSystemTray = true;

    QRegExp all("^-(t|-tray|h|-help)$");
    QStringList others;
    foreach (QString arg, cmdlineArgs) if (all.indexIn(arg)==-1) others.append(arg);
    if (others.size() > 1) return false;

    if (others.size()==1) filePath = others.first();

    return true;
}


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qtmindmap);

    QApplication a(argc, argv);

    bool isSystemTray(false);
    QString filePath;
    if (!parseCmdLineArgs(isSystemTray,filePath))
    {
        printUsage();
        return EXIT_FAILURE;
    }

    MainWindow w;
    SystemTray *systemTray;
    isSystemTray = true;
    if (isSystemTray)
    {
        if (!QSystemTrayIcon::isSystemTrayAvailable())
        {
            QMessageBox::critical(0,
                                  QObject::tr("Systray"),
                                  QObject::tr("I couldn't detect any system tray on this system."));
            return EXIT_FAILURE;
        }
        QApplication::setQuitOnLastWindowClosed(false);
        systemTray = new SystemTray(&w);
        systemTray->show();
    }


    w.show();
    return a.exec();
}
