#include <stdlib.h> // EXIT_SUCCESS
#include <iostream> // cout

#include <QtGui>
#include <QDebug>
#include <QRegExp>
#include <QTranslator>

#include "mainwindow.h"
#include "systemtray.h"
#include "argumentparser.h"





int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qtmindmap);

    QApplication a(argc, argv);

    // translation
    QString locale = QLocale::system().name();
    QTranslator translator;
    if (!translator.load(QString("qtmindmap_") + locale))
    {
        std::cerr << "No translation file for locale: " << locale.toStdString() << std::endl;
    }
    else
    {
        a.installTranslator(&translator);
    }

    // parse args
    ArgumentParser argParser;
    bool success;
    if (!argParser.parseCmdLineArgs(success))
    {
        return success ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    // system tray?
    MainWindow w;
    SystemTray systemtray(&w);
    if (argParser.isSystemTray() or argParser.isShowMinimized())
    {
        if (!QSystemTrayIcon::isSystemTrayAvailable())
        {
            QMessageBox::critical(0,
                                  QObject::tr("QtMindMap Error"),
                                  QObject::tr("I couldn't detect any system tray on this system."));
            return EXIT_FAILURE;
        }
        QApplication::setQuitOnLastWindowClosed(false);
        systemtray.setup();
        systemtray.show();
    }

    if (!argParser.isShowMinimized()) w.show();
    return a.exec();
}
