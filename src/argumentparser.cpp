#include "include/argumentparser.h"

#include <QApplication>
#include <QStringList>
#include <QFileInfo>

#include <iostream>

void ArgumentParser::printUsage()
{
    std::cout << tr("Usage: ").toStdString() << "qtmindmap "
              << tr("[OPTION][FILE]").toStdString()
              << std::endl
              << tr("Mindmap program in QT").toStdString() << std::endl
              << std::endl
              << tr("Options:").toStdString() << std::endl
              << "-h,  --help\t\t" << tr("Prints this help.").toStdString()
              << std::endl
              << "-t,  --tray\t\t"
              << tr("Starts application in system tray.").toStdString()
              << std::endl
              << "-s,  --show-minimized\t"
              << tr("Hide main window, just show systray icon.").toStdString()
              << std::endl << std::endl
              << tr("Report bugs to: ").toStdString()
              << "denes.matetelki@gmail.com" << std::endl;
}


bool ArgumentParser::parseCmdLineArgs(bool &successful)
{
    QStringList cmdlineArgs = QCoreApplication::arguments();
    cmdlineArgs.removeFirst();

    QRegExp help("^-(h|-help)$");
    if (!cmdlineArgs.filter(help).isEmpty())
    {
        printUsage();
        successful = true;
        return false;
    }

    QRegExp tray("^-(t|-tray)$");
    if (!cmdlineArgs.filter(tray).isEmpty())
        m_isSystemTray = true;

    QRegExp minimized("^-(s|-show-minimized)$");
    if (!cmdlineArgs.filter(minimized).isEmpty())
        m_isShowMinimized = true;

    /// @note It is an error? Shall it be handled?
    // if (isSystemTray && isShowMinimized) return false;

    QRegExp all("^-(t|-tray|h|-help|s|-show-minimized)$");
    QStringList others;
    foreach (QString arg, cmdlineArgs)
        if (all.indexIn(arg)==-1)
            others.append(arg);

    if (others.size() > 1)
    {
        std::cerr << tr("Unkown options: ").toStdString()
                  << others.join(" ").toStdString() << std::endl;
        printUsage();
        successful = false;
        return false;
    }

    if (others.size()==1)
    {
        if (others.first().at(0)=='-')
        {
            successful = false;
            return false;
        }

        /// @note filecheck shall  be done elsewhere?
        m_filePath = others.first();
        QFileInfo fileInfo(m_filePath);
        if (!fileInfo.exists())
        {
            std::cerr << tr("File: ").toStdString() <<
                         m_filePath.toStdString() <<
                         tr(" does not exists.").toStdString() << std::endl;
            successful = false;
            return false;
        }
        if (!fileInfo.isFile())
        {
            std::cerr << tr("File: ").toStdString() <<
                         m_filePath.toStdString() <<
                         tr(" is not a file.").toStdString() << std::endl;
            successful = false;
            return false;
        }
        if (!fileInfo.isReadable())
        {
            std::cerr << tr("File: ").toStdString() <<
                         m_filePath.toStdString() <<
                         tr(" is not readable.").toStdString() << std::endl;
            successful = false;
            return false;
        }
    }
    return true;
}
