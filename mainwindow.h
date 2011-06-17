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
//    QStatusBar * getStatusBar(); /// rewrite as a message slot?
    void statusBarMsg(const QString &msg);

public slots:

    void contentChanged(const bool &changed = true);

    void newFile();
    void openFile(const QString &fileName = "");
    void saveFile();
    void saveFileAs();
    void closeFile();

    void exportScene();
    void about();

private:

    void setTitle(const QString &title);

    Ui::MainWindow *m_ui;
    GraphWidget *m_graphicsView;
    QString m_fileName;
    bool m_contentChanged;

};

#endif // MAINWINDOW_H
