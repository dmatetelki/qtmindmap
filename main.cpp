#include <stdlib.h> // EXIT_SUCCESS
#include <iostream> // cout

#include <QtGui>
#include <QDebug>
#include <QRegExp>
#include <QTranslator>

#include "mainwindow.h"


void printUsage()
{
    std::cout << "Usage: qtmindmap [OPTION][FILE]" << std::endl
              << "Mindmap program in QT" << std::endl
              << std::endl
              << "Options:" << std::endl
              << "-h,  --help\tPrints this help." << std::endl
              << "-t,  --tray\tStarts application in system tray." << std::endl
              << "-s,  --show-minimized\tHide main window, just show systray icon." << std::endl
              << std::endl
              << "Report bugs to: denes.matetelki@gmail.com" << std::endl;
}


bool parseCmdLineArgs(bool &isSystemTray,
                      bool &isShowMinimized,
                      QString &filePath)
{
    QStringList cmdlineArgs = QCoreApplication::arguments();
    cmdlineArgs.removeFirst();

    QRegExp help("^-(h|-help)$");
    if (!cmdlineArgs.filter(help).isEmpty()) return false;

    QRegExp tray("^-(t|-tray)$");
    if (!cmdlineArgs.filter(tray).isEmpty()) isSystemTray = true;

    QRegExp minimized("^-(s|-show-minimized)$");
    if (!cmdlineArgs.filter(minimized).isEmpty()) isShowMinimized = true;

    /// @note It is an error? Shall it be handled?
    // if (isSystemTray && isShowMinimized) return false;

    QRegExp all("^-(t|-tray|h|-help|s|-show-minimized)$");
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

    // translation
    QString locale = QLocale::system().name();
    QTranslator translator;
    if (!translator.load(QString("qtmindmap_") + locale))
    {
        std::cout << "No translation file for locale: " << locale.toStdString() << std::endl;
    }
    else
    {
        a.installTranslator(&translator);
    }

    // parse args
    bool isSystemTray(false);
    bool isShowMinimized(false);
    QString filePath;
    if (!parseCmdLineArgs(isSystemTray,isShowMinimized,filePath))
    {
        printUsage();
        return EXIT_FAILURE;
    }

    // system tray?
    MainWindow w(isSystemTray or isShowMinimized);
    if (isSystemTray or isShowMinimized)
    {
        if (!QSystemTrayIcon::isSystemTrayAvailable())
        {
            QMessageBox::critical(0,
                                  QObject::tr("QtMindMap Error"),
                                  QObject::tr("I couldn't detect any system tray on this system."));
            return EXIT_FAILURE;
        }
        QApplication::setQuitOnLastWindowClosed(false);
        w.showSysTray();
    }

    if (!isShowMinimized) w.show();
    return a.exec();
}
