#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_contentChanged(false)
{
    m_graphicsView = new GraphWidget(this);

    m_ui->setupUi(this);
    connect(m_ui->actionNew, SIGNAL(activated()),
            this, SLOT(newFile()));
    connect(m_ui->actionOpen, SIGNAL(activated()),
            this, SLOT(openFile()));
    connect(m_ui->actionSave, SIGNAL(activated()),
            this, SLOT(saveFile()));
    connect(m_ui->actionSaveAs, SIGNAL(activated()),
            this, SLOT(saveFileAs()));
    connect(m_ui->actionClose, SIGNAL(activated()),
            this, SLOT(closeFile()));
    connect(m_ui->actionExport, SIGNAL(activated()),
            this, SLOT(exportScene()));

    connect(m_ui->actionQuit, SIGNAL(activated()),
            QApplication::instance(), SLOT(quit()));

    connect(m_ui->actionAbout_QtMindMap, SIGNAL(activated()),
            this, SLOT(about()));
    connect(m_ui->actionKeys, SIGNAL(activated()),
            this, SLOT(keys()));


    setCentralWidget(m_graphicsView);
    m_graphicsView->hide();

    // why can't I do this with qtcreator?
    /// @bug or a feature? no underline here
    m_trash = new QAction(QIcon(":/user-trash-full.svg"), "&Trash it", this);
    m_trash->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    m_ui->mainToolBar->addAction(m_trash);

    m_info = new QAction(QIcon(":/folder.svg"), "&Reference it", this);
    m_info->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    m_ui->mainToolBar->addAction(m_info);

    m_blocked = new QAction(QIcon(":/dialog-warning.svg"), tr("&Blocked"), this);
    m_blocked->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
    m_ui->mainToolBar->addAction(m_blocked);

    m_question = new QAction(QIcon(":/help-browser.svg"), tr("&What shall be done?"), this);
//    m_question->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    m_ui->mainToolBar->addAction(m_question);

    m_postpone = new QAction(QIcon(":/x-office-calendar.svg"), tr("&Postpone it"), this);
    m_postpone->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    m_ui->mainToolBar->addAction(m_postpone);

    m_delegate = new QAction(QIcon(":/system-users.svg"), tr("&Delegate it"), this);
    m_delegate->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
    m_ui->mainToolBar->addAction(m_delegate);

    m_ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
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
    if (m_contentChanged == false && changed == true)
    {
        setWindowTitle(windowTitle().prepend("* "));
        m_contentChanged = true;
        if (m_fileName != "untitled")
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
    contentChanged(false);
    m_fileName = "untitled";
    setTitle(m_fileName);

    m_graphicsView->setFocus();
}

void MainWindow::openFile(const QString &fileName)
{
    if (!closeFile())
        return;

    if (fileName.isEmpty())
    {
        QFileDialog dialog(this,
                           tr("Open MindMap"),
                           QDir::homePath(),
                           QString("QtMindMap (*.qmm)"));
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setDefaultSuffix("qmm");

        if (dialog.exec())
        {
            m_fileName = dialog.selectedFiles().first();
        }
        else
        {
            return;
        }
    }
    else
    {
        m_fileName = fileName;
    }

    m_graphicsView->readContentFromXmlFile(m_fileName);

    m_ui->actionSave->setEnabled(true);
    m_ui->actionSaveAs->setEnabled(true);
    m_ui->actionClose->setEnabled(true);
    contentChanged(false);
    setTitle(m_fileName);
}

void MainWindow::saveFile()
{
    m_graphicsView->writeContentToXmlFile(m_fileName);
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

    if (dialog.exec())
    {
        m_fileName = dialog.selectedFiles().first();
        setTitle(m_fileName);
        saveFile();
        return true;
    }
    else
    {
        return false; // cancelled
    }
}

bool MainWindow::closeFile()
{
    if (m_contentChanged)
    {
        QMessageBox msgBox(this);
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save |
                                  QMessageBox::Discard |
                                  QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Save:
        {
            if (m_fileName == "untitled")
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
    m_contentChanged = false;
    setTitle("");
    m_graphicsView->closeScene();
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
         m_graphicsView->writeContentToPngFile(dialog.selectedFiles().first());
     }
}

void MainWindow::keys()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Keys of QtMindMap"));
    msgBox.setText(tr("Usage of keys & mouse buttons"));
    msgBox.setInformativeText(

                QString("<table><tr><td><b>").

                append(tr("Keys")).
                append("</b></td><td></td></tr><tr><td>+,-</td><td>").
                append(tr("zoom in/out of the view")).
                append("</td></tr></tr><tr><td>").
                append(tr("cursors")).
                append("</td><td>").
                append(tr("move view scrollbars")).
                append("</td></tr></tr><tr><td>").
                append(tr("ctrl + cursors")).
                append("</td><td>").
                append(tr("move active node")).
                append("</td></tr></tr><tr><td>Delete</td><td>").
                append(tr("remove active node")).
                append("</td></tr></tr><tr><td>Insert</td><td>").
                append(tr("add new node to active node")).
                append("</td></tr></tr><tr><td>f</td><td>").
                append(tr("enter/leave hint mode (vimperator style select)")).
                append("</td></tr></tr><tr><td>").
                append(tr("numbers, backspace, enter")).
                append("</td><td>").
                append(tr("select numbers in hint mode")).
                append("</td></tr></tr><tr><td>F2</td><td>").
                append(tr("edit active node")).
                append("</td></tr></tr><tr><td>Esc</td><td>").
                append(tr("leaving node editing/adding/deleting mode")).
                append("</td></tr></tr><tr><td>").
                append(tr("editing mode: ctrl+cursors")).
                append("</td><td>").
                append(tr("jump to beginning of next/prev word")).
                append("</td></tr></tr><tr><td>a</td><td>").
                append(tr("add new edge to active node (select the destination)")).
                append("</td></tr></tr><tr><td>d</td><td>").
                append(tr("delete edge of active node (select other end)")).
                append("</td></tr><tr><td><b>").
                append(tr("Mouse")).
                append("</b></td><td></td></tr><tr><td>").
                append(tr("scroll")).
                append("</td><td>").
                append(tr("zoom in/out of the view")).
                append("</td></tr><tr><td>").
                append(tr("click")).
                append("</td><td>").
                append(tr("select node")).
                append("</td></tr><tr><td>").
                append(tr("double clikk")).
                append("</td><td>").
                append(tr("set node editable")).
                append("</td></tr><tr><td>").
                append(tr("drag")).
                append("</td><td>").
                append(tr("move node")).
                append("</td></tr></table>"));

    msgBox.exec();
}

void MainWindow::about()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("About QtMindMap"));
    msgBox.setText(tr("MindMap software written in Qt."));
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setInformativeText(tr("Homepage:").
                              append(" <a href=\"https://gitorious.org/qtmindmap\">https://gitorious.org/qtmindmap</a><br><br>").
                              append(tr("Report bugs to:")).
                              append(" <a href=\"mailto:denes.matetelki@gmail.com\">denes.matetelki@gmail.com</a>"));
    QPixmap pixMap(":/qtmindmap.svg");
    msgBox.setIconPixmap(pixMap.scaled(50,50));
    msgBox.exec();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    m_contentChanged && !closeFile() ? event->ignore() : event->accept();
}

void MainWindow::setTitle(const QString &title)
{
    if (title.isEmpty())
    {
        setWindowTitle("QtMindMap");
    }
    else
    {
        QString t(title);
        t.append(" - QtMindMap");
        setWindowTitle(t);
    }
}
