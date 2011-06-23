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

    void statusBarMsg(const QString &msg);
    void contentChanged(const bool &changed = true);

public slots:

    void newFile();
    void openFile(const QString &fileName = "");
    void saveFile();
    bool saveFileAs();
    bool closeFile();
    void exportScene();

    void keys();
    void about();

    void insertPicture();

protected:
    void closeEvent(QCloseEvent * event);

private:

    void setTitle(const QString &title);

    Ui::MainWindow *m_ui;
    GraphWidget *m_graphicsView;
    QString m_fileName;
    bool m_contentChanged;

    QAction *m_doIt;
    QAction *m_trash;
    QAction *m_info;
    QAction *m_blocked;
    QAction *m_question;
    QAction *m_postpone;
    QAction *m_delegate;
};

#endif // MAINWINDOW_H
