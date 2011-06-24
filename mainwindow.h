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

    void about();

    void insertPicture();

    void showMainToolbar(const bool &show = true);
    void showStatusIconToolbar(const bool &show = true);


protected:
    void closeEvent(QCloseEvent * event);
    void keyPressEvent(QKeyEvent *event);

private:

    void setTitle(const QString &title);

    Ui::MainWindow *m_ui;
    GraphWidget *m_graphicsView;
    QString m_fileName;
    bool m_contentChanged;

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

    QAction *m_insertIcon;
    QAction *m_doIt;
    QAction *m_trash;
    QAction *m_info;
    QAction *m_blocked;
    QAction *m_question;
    QAction *m_postpone;
    QAction *m_delegate;
    QAction *m_maybe;
};

#endif // MAINWINDOW_H
