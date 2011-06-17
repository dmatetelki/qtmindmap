#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "graphwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     QStatusBar * getStatusBar();
     QString getFileName() { return m_fileName; }
     void openFile(QString fileName);
     void enableSave(const bool &enable = true);
     void enableSaveAs(const bool &enable = true);
     void enableCloseFile(const bool &enable = true);
     void setTitle(const QString &title);
//     void setModifiedTitle(const bool &modified = true);

public slots:

    void exportScene();
    void about();

private:

    Ui::MainWindow *m_ui;
    GraphWidget *m_graphicsView;
    QString m_fileName;

};

#endif // MAINWINDOW_H
