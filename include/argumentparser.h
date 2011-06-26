#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <QObject>

class ArgumentParser : public QObject
{
    Q_OBJECT

public:

    explicit ArgumentParser(QObject *parent = 0)
        : QObject(parent)
        , m_isSystemTray(false)
        , m_isShowMinimized(false)
        , m_filePath() {}

    /** parse QCoreApplication::arguments and put data to priv. members
      * @return true if the program can continue
      */
    bool parseCmdLineArgs();

    bool isSystemTray();
    bool isShowMinimized();
    QString filePath();


private:

    // print --help text
    void printUsage();

    bool m_isSystemTray;
    bool m_isShowMinimized;
    QString m_filePath;
};

#endif // ARGUMENTPARSER_H
