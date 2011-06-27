#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QSignalMapper>

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

    // instead of givin access to private m_ui
    void statusBarMsg(const QString &msg);

    // indicate that content has changed, modify title, save actions
    void contentChanged(const bool &changed = true);

public slots:

    // filemenu actions
    void newFile();
    void openFile(const QString &fileName = "");
    void saveFile(const bool &checkIfReadonly = true);
    bool saveFileAs();
    bool closeFile();
    void exportScene();
    void about();

    // toolbars
    void showMainToolbar(const bool &show = true);
    void showStatusIconToolbar(const bool &show = true);

    // handle changed content at quit
    void quit();

protected:

    // handle changed content at exit events
    void closeEvent(QCloseEvent * event);

    // show/hide toolbars, otherwise pass on the event to GraphWidget
    void keyPressEvent(QKeyEvent *event);

private:

    void setUpMainToolbar();
    void setUpStatusIconToolbar();
    void setTitle(const QString &title);

    Ui::MainWindow *m_ui;
    GraphWidget *m_graphicsView;
    QString m_fileName;
    bool m_contentChanged;

    // main toolbar actions
    QAction *m_addNode;
    QAction *m_delNode;
    QAction *m_editNode;
    QAction *m_scaleUpNode;
    QAction *m_scaleDownNode;
    QAction *m_nodeColor;
    QAction *m_nodeTextColor;
    QAction *m_addEdge;
    QAction *m_delEdge;
    QAction *m_zoomIn;
    QAction *m_zoomOut;
    QAction *m_esc;
    QAction *m_hintMode;
    QAction *m_moveNode;
    QAction *m_subtree;
    QAction *m_showMainToolbar;
    QAction *m_showStatusIconToolbar;

    // state icons toolbar actions
    QAction *m_insertIcon;
    QAction *m_doIt;
    QAction *m_trash;
    QAction *m_info;
    QAction *m_blocked;
    QAction *m_question;
    QAction *m_postpone;
    QAction *m_delegate;
    QAction *m_maybe;
    QSignalMapper *m_signalMapper;
};

#endif // MAINWINDOW_H
