#include "include/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_contentChanged(false)
{
    // setup ui
    m_ui->setupUi(this);
    connect(m_ui->actionNew, SIGNAL(activated()), this, SLOT(newFile()));
    connect(m_ui->actionOpen, SIGNAL(activated()), this, SLOT(openFile()));
    connect(m_ui->actionSave, SIGNAL(activated()), this, SLOT(saveFile()));
    connect(m_ui->actionSaveAs, SIGNAL(activated()), this, SLOT(saveFileAs()));
    connect(m_ui->actionClose, SIGNAL(activated()), this, SLOT(closeFile()));
    connect(m_ui->actionExport, SIGNAL(activated()), this, SLOT(exportScene()));
    connect(m_ui->actionQuit, SIGNAL(activated()), this, SLOT(quit()));
    connect(m_ui->actionAbout_QtMindMap, SIGNAL(activated()),
            this, SLOT(about()));

    // graphwidget is hided by def, new/open file will show it
    m_graphicsView = new GraphWidget(this);
    setCentralWidget(m_graphicsView);
    m_graphicsView->hide();

    connect(m_graphicsView->graphLogic(), SIGNAL(contentChanged(const bool&)),
            this, SLOT(contentChanged(const bool&)));

    connect(m_graphicsView->graphLogic(), SIGNAL(notification(QString)),
            this, SLOT(statusBarMsg(QString)));

    // setup toolbars, don't show them
    setupMainToolbar();
    m_ui->mainToolBar->hide();

    setupStatusIconToolbar();
    m_ui->statusIcons_toolBar->hide();

    setupEditToolbar();
    m_ui->undoToolBar->hide();
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::statusBarMsg(const QString &msg)
{
    m_ui->statusBar->showMessage(msg, 5000);
}

void MainWindow::contentChanged(const bool& changed)
{
    // only care about the transitions
    if (m_contentChanged == false && changed == true)
    {
        setWindowTitle(windowTitle().prepend("* "));
        m_contentChanged = true;

        QFileInfo fileInfo(m_fileName);
        if (m_fileName != tr("untitled") && fileInfo.isWritable())
            m_ui->actionSave->setEnabled(true);
    }
    else if (m_contentChanged == true && changed == false)
    {
        setWindowTitle(windowTitle().remove(0,2));
        m_contentChanged = false;
        m_ui->actionSave->setEnabled(false);
    }
}

void MainWindow::newFile()
{
    if (!closeFile())
        return;

    m_graphicsView->newScene();

    m_ui->actionSave->setEnabled(false);
    m_ui->actionSaveAs->setEnabled(true);
    m_ui->actionClose->setEnabled(true);
    m_ui->actionExport->setEnabled(true);
    contentChanged(false);
    m_fileName = tr("untitled");
    setTitle(m_fileName);

    m_graphicsView->setFocus();

    showMainToolbar();
}

void MainWindow::openFile(const QString &fileName)
{
    if (!closeFile())
        return;

    QString currFilename(m_fileName);
    if (fileName.isEmpty())
    {
        QFileDialog dialog(this,
                           tr("Open MindMap"),
                           QDir::homePath(),
                           QString("QtMindMap (*.qmm)"));
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setDefaultSuffix("qmm");

        if (!dialog.exec())
            return;

        m_fileName = dialog.selectedFiles().first();
    }
    else
    {
        m_fileName = fileName;
    }

    QFileInfo fileInfo(m_fileName);
    if (!fileInfo.isWritable())
        statusBarMsg(tr("Read-only file!"));

    if (!m_graphicsView->graphLogic()->readContentFromXmlFile(m_fileName))
    {
        m_fileName = currFilename;
        return;
    }


    m_ui->actionSaveAs->setEnabled(true);
    m_ui->actionClose->setEnabled(true);
    m_ui->actionExport->setEnabled(true);
    m_ui->actionSave->setEnabled(false);
    m_ui->actionSave->setEnabled(false);

    contentChanged(false);

    fileInfo.isWritable() ?
         setTitle(m_fileName) :
         setTitle(tr("readonly ").append(m_fileName));

    showMainToolbar();
}

void MainWindow::saveFile(const bool &checkIfReadonly)
{
    QFileInfo fileInfo(m_fileName);
    if (checkIfReadonly && !fileInfo.isWritable())
    {
        statusBarMsg(tr("Read-only file!"));
        return;
    }

    m_graphicsView->graphLogic()->writeContentToXmlFile(m_fileName);
    contentChanged(false);
}

bool MainWindow::saveFileAs()
{
    QFileDialog dialog(this,
                       tr("Save MindMap as"),
                       QDir::homePath(),
                       QString("QtMindMap (*.qmm)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("qmm");

    if (!dialog.exec())
        return false;

    m_fileName = dialog.selectedFiles().first();
    saveFile(false);
    setTitle(m_fileName);
    return true;
}

bool MainWindow::closeFile()
{
    if (m_contentChanged)
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Save mindmap - qtmindmap"));
        msgBox.setText(tr("The mindmap has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save |
                                  QMessageBox::Discard |
                                  QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Save:
        {
            if (m_fileName == tr("untitled"))
            {
                if (!saveFileAs())
                        return false;
            }
            else
            {
                saveFile();
            }

            break;
        }
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            return false;
        default:
            break;
        }
    }

    m_ui->actionSave->setEnabled(false);
    m_ui->actionSaveAs->setEnabled(false);
    m_ui->actionClose->setEnabled(false);
    m_ui->actionExport->setEnabled(false);
    m_contentChanged = false;
    setTitle("");
    m_graphicsView->closeScene();
    showMainToolbar(false);
    return true;
}

void MainWindow::exportScene()
{
    QFileDialog dialog(this,
                       tr("Export MindMap to image"),
                       QDir::homePath(),
                       tr("PNG image file (*.png)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("png");

     if (dialog.exec())
     {
         m_graphicsView->graphLogic()->writeContentToPngFile(
                                            dialog.selectedFiles().first());
     }
}

void MainWindow::about()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("About QtMindMap"));
    msgBox.setText(tr("MindMap software written in Qt."));
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setInformativeText(tr("Homepage:").
                    append(" <a href=\"https://gitorious.org/qtmindmap\">" \
                           "https://gitorious.org/qtmindmap</a><br><br>").
                    append(tr("Report bugs to:")).
                    append(" <a href=\"mailto:denes.matetelki@gmail.com\">" \
                           "denes.matetelki@gmail.com</a>"));
    QPixmap pixMap(":/qtmindmap.svg");
    msgBox.setIconPixmap(pixMap.scaled(50,50));
    msgBox.exec();
}

void MainWindow::showMainToolbar(const bool &show)
{
    m_ui->mainToolBar->setVisible(show ?
                                      !m_ui->mainToolBar->isVisible() :
                                      false);
}

void MainWindow::showStatusIconToolbar(const bool &show)
{
    m_ui->statusIcons_toolBar->setVisible(
                                show ?
                                     !m_ui->statusIcons_toolBar->isVisible() :
                                     false);
}

void MainWindow::showUdoToolbar(const bool &show)
{
    m_ui->undoToolBar->setVisible(show ?
                                     !m_ui->undoToolBar->isVisible() :
                                     false);
}

void MainWindow::quit()
{
    if (m_contentChanged && !closeFile())
        return;

    QApplication::instance()->quit();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    m_contentChanged && !closeFile() ?
                event->ignore() :
                event->accept();
}

void MainWindow::setupMainToolbar()
{
    // why can't I do this with qtcreator? (adding actions to toolbar)

    /// @bug or a feature? no underline here
    m_addNode = new QAction(tr("Add node (ins)"), this);
    connect(m_addNode, SIGNAL(activated()), m_graphicsView->graphLogic(),
            SLOT(insertNode()));

    m_delNode = new QAction(tr("Del node (del)"), this);
    connect(m_delNode, SIGNAL(activated()), m_graphicsView->graphLogic(),
            SLOT(removeNode()));

    m_editNode = new QAction(tr("Edit node (F2, dubclick)"), this);
    connect(m_editNode, SIGNAL(activated()), m_graphicsView->graphLogic(),
            SLOT(nodeEdited()));

    m_scaleUpNode = new QAction(tr("ScaleUp Node (Ctrl +)"), this);
    connect(m_scaleUpNode, SIGNAL(activated()), m_graphicsView->graphLogic(),
            SLOT(scaleUp()));
    m_scaleDownNode = new QAction(tr("ScaleDown Node (Ctrl -)"), this);
    connect(m_scaleDownNode, SIGNAL(activated()), m_graphicsView->graphLogic(),
            SLOT(scaleDown()));

    m_nodeColor = new QAction(tr("Node color (c)"), this);
    connect(m_nodeColor, SIGNAL(activated()), m_graphicsView->graphLogic(),
            SLOT(nodeColor()));

    m_nodeTextColor = new QAction(tr("Node textcolor (t)"), this);
    connect(m_nodeTextColor, SIGNAL(activated()), m_graphicsView->graphLogic(),
            SLOT(nodeTextColor()));

    m_addEdge = new QAction(tr("Add edge (a)"), this);
    connect(m_addEdge, SIGNAL(activated()), m_graphicsView->graphLogic(),
            SLOT(addEdge()));

    m_delEdge = new QAction(tr("Del edge (d)"), this);
    connect(m_delEdge, SIGNAL(activated()), m_graphicsView->graphLogic(),
            SLOT(removeEdge()));

    m_moveNode = new QAction(tr("Move node\n(Ctrl cursor, drag)"), this);
    m_moveNode->setDisabled(true);

    m_subtree = new QAction(tr("Change on wholesubtree\n(Ctrl shift)"), this);
    m_subtree->setDisabled(true);

    m_zoomIn = new QAction(tr("Zoom in (+, scrollup)"), this);
    connect(m_zoomIn, SIGNAL(activated()), m_graphicsView,
            SLOT(zoomIn()));

    m_zoomOut = new QAction(tr("Zoom out (-, scrolldown)"), this);
    connect(m_zoomOut, SIGNAL(activated()), m_graphicsView,
            SLOT(zoomOut()));

    m_esc = new QAction(tr("Leave editing,\nedge eadd/remove (esc)"), this);
    connect(m_esc, SIGNAL(activated()), m_graphicsView->graphLogic(),
            SLOT(nodeLostFocus()));

    m_hintMode = new QAction(tr("Hint mode (f)"), this);
    connect(m_hintMode, SIGNAL(activated()), m_graphicsView->graphLogic(),
            SLOT(hintMode()));

    m_showMainToolbar = new QAction(tr("Show main toolbar\n(Ctrl m)"), this);
    connect(m_showMainToolbar, SIGNAL(activated()), this,
            SLOT(showMainToolbar()));

    m_showStatusIconToolbar = new QAction(tr("Insert status icons\n(Ctrl i)"),
                                          this);
    connect(m_showStatusIconToolbar, SIGNAL(activated()), this,
            SLOT(showStatusIconToolbar()));

    m_ui->mainToolBar->addAction(m_addNode);
    m_ui->mainToolBar->addAction(m_delNode);
    m_ui->mainToolBar->addAction(m_editNode);
    m_ui->mainToolBar->addAction(m_scaleUpNode);
    m_ui->mainToolBar->addAction(m_scaleDownNode);
    m_ui->mainToolBar->addAction(m_nodeColor);
    m_ui->mainToolBar->addAction(m_nodeTextColor);
    m_ui->mainToolBar->addAction(m_addEdge);
    m_ui->mainToolBar->addAction(m_delEdge);

    m_ui->mainToolBar->addSeparator();
    m_ui->mainToolBar->addAction(m_zoomIn);
    m_ui->mainToolBar->addAction(m_zoomOut);
    m_ui->mainToolBar->addAction(m_esc);
    m_ui->mainToolBar->addAction(m_hintMode);
    m_ui->mainToolBar->addAction(m_moveNode);
    m_ui->mainToolBar->addAction(m_subtree);
    m_ui->mainToolBar->addAction(m_showMainToolbar);
    m_ui->mainToolBar->addAction(m_showStatusIconToolbar);
}

void MainWindow::setupStatusIconToolbar()
{
    // map signals so actions can send icon name
    m_signalMapper = new QSignalMapper(this);

    m_insertIcon = new QAction(tr("Insert icon:"), this);
    m_insertIcon->setDisabled(true);

    m_doIt = new QAction(QIcon(":/applications-system.svg"), tr("&Do"), this);
    m_doIt->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
    connect(m_doIt, SIGNAL(activated()), m_signalMapper, SLOT (map()));
    m_signalMapper->setMapping(m_doIt, ":/applications-system.svg");

    m_trash = new QAction(QIcon(":/user-trash-full.svg"), tr("&Trash"), this);
    m_trash->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    connect(m_trash, SIGNAL(activated()), m_signalMapper, SLOT (map()));
    m_signalMapper->setMapping(m_trash, ":/user-trash-full.svg");

    m_info = new QAction(QIcon(":/mail-attachment.svg"), tr("&Refer"), this);
    m_info->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    connect(m_info, SIGNAL(activated()), m_signalMapper, SLOT (map()));
    m_signalMapper->setMapping(m_info, ":/mail-attachment.svg");

    m_blocked = new QAction(QIcon(":/dialog-warning.svg"), tr("&Blocked"),
                            this);
    m_blocked->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
    connect(m_blocked, SIGNAL(activated()), m_signalMapper, SLOT (map()));
    m_signalMapper->setMapping(m_blocked, ":/dialog-warning.svg");

    m_question = new QAction(QIcon(":/help-browser.svg"), tr("&How?"), this);
    m_question->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
    connect(m_question, SIGNAL(activated()), m_signalMapper, SLOT (map()));
    m_signalMapper->setMapping(m_question, ":/help-browser.svg");

    m_postpone = new QAction(QIcon(":/x-office-calendar.svg"), tr("&Postpone"),
                             this);
    m_postpone->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    connect(m_postpone, SIGNAL(activated()), m_signalMapper, SLOT (map()));
    m_signalMapper->setMapping(m_postpone, ":/x-office-calendar.svg");

    m_delegate = new QAction(QIcon(":/system-users.svg"), tr("&Comission"),
                             this);
    m_delegate->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    connect(m_delegate, SIGNAL(activated()), m_signalMapper, SLOT (map()));
    m_signalMapper->setMapping(m_delegate, ":/system-users.svg");

    m_maybe = new QAction(QIcon(":/dialog-information.svg"), tr("ma&Ybe"),
                          this);
    m_maybe->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
    connect(m_maybe, SIGNAL(activated()), m_signalMapper, SLOT (map()));
    m_signalMapper->setMapping(m_maybe, ":/dialog-information.svg");

    connect(m_signalMapper, SIGNAL(mapped(const QString &)),
            m_graphicsView->graphLogic(), SLOT(insertPicture(const QString &)));

    m_ui->statusIcons_toolBar->addAction(m_insertIcon);
    m_ui->statusIcons_toolBar->addAction(m_doIt);
    m_ui->statusIcons_toolBar->addAction(m_trash);
    m_ui->statusIcons_toolBar->addAction(m_info);
    m_ui->statusIcons_toolBar->addAction(m_blocked);
    m_ui->statusIcons_toolBar->addAction(m_question);
    m_ui->statusIcons_toolBar->addAction(m_postpone);
    m_ui->statusIcons_toolBar->addAction(m_delegate);
    m_ui->statusIcons_toolBar->addAction(m_maybe);
    m_ui->statusIcons_toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void MainWindow::setupEditToolbar()
{
    m_undoStack = new QUndoStack(this);
    m_undoView = new QUndoView(m_undoStack,this);
    m_ui->undoToolBar->addWidget(m_undoView);

    m_undo = m_undoStack->createUndoAction(this, tr("&Undo"));
    m_undo->setShortcuts(QKeySequence::Undo);
    m_redo = m_undoStack->createRedoAction(this, tr("&Redo"));
    m_redo->setShortcuts(QKeySequence::Redo);
    m_ui->menuEdit->addAction(m_undo);
    m_ui->menuEdit->addAction(m_redo);

    m_ui->menuEdit->addSeparator();

    m_mainToolbar = new QAction(tr("main toolbar"), this);
    m_mainToolbar->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    connect(m_mainToolbar, SIGNAL(activated()),
            this, SLOT (showMainToolbar()));

    m_iconToolbar = new QAction(tr("icon toolbar"), this);
    m_iconToolbar->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
    connect(m_iconToolbar, SIGNAL(activated()),
            this, SLOT (showStatusIconToolbar()));

    m_undoToolbar = new QAction(tr("undo toolbar"), this);
    m_undoToolbar->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
    connect(m_undoToolbar, SIGNAL(activated()),
            this, SLOT (showUdoToolbar()));

    m_ui->menuEdit->addAction(m_mainToolbar);
    m_ui->menuEdit->addAction(m_iconToolbar);
    m_ui->menuEdit->addAction(m_undoToolbar);

    m_graphicsView->graphLogic()->setUndoStack(m_undoStack);
}

void MainWindow::setTitle(const QString &title)
{
    title.isEmpty() ?
         setWindowTitle("QtMindMap") :
         setWindowTitle(QString(title).append(" - QtMindMap"));
}
