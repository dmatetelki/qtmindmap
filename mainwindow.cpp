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

    setCentralWidget(m_graphicsView);
    m_graphicsView->hide();
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
                           tr("QtMindMap (*.qmm)"));
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
                       tr("QtMindMap (*.qmm)"));
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

void MainWindow::about()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("About QtMindMap"));
    msgBox.setText(tr("MindMap software written in Qt."));
    msgBox.setInformativeText(tr("Homepage: ").
                              append("https://gitorious.org/qtmindmap\n\n").
                              append(tr("Report bugs to: ")).
                              append("denes.matetelki@gmail.com"));
    QPixmap pixMap(":/qtmindmap.svg");
    msgBox.setIconPixmap(pixMap.scaled(50,50));
    msgBox.exec();
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
