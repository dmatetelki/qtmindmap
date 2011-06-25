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
      * @param successful true if cannot continue but it is not an error
      * @return true if the program can continue
      */
    bool parseCmdLineArgs(bool &successful);

    bool isSystemTray() { return m_isSystemTray; }
    bool isShowMinimized() { return m_isShowMinimized; }
    QString filePath() { return m_filePath; }


private:

    // print --help text
    void printUsage();

    bool m_isSystemTray;
    bool m_isShowMinimized;
    QString m_filePath;
};

#endif // ARGUMENTPARSER_H
