#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <QObject>

class ArgumentParser : public QObject
{
    Q_OBJECT

public:

    explicit ArgumentParser(QObject *parent = 0);

    bool parseCmdLineArgs(bool &successful);

    bool isSystemTray() { return m_isSystemTray; }
    bool isShowMinimized() { return m_isShowMinimized; }
    QString filePath() { return m_filePath; }


private:

    void printUsage();

    bool m_isSystemTray;
    bool m_isShowMinimized;
    QString m_filePath;
};

#endif // ARGUMENTPARSER_H
