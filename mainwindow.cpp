#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

//#include <QtConcurrentRun>

/// QPixmap: It is not safe to use pixmaps outside the GUI thread
/// tr is messy
/*
extern void exportScaneToPng(QGraphicsScene *scene,
                             const QString &fileName,
                             Ui::MainWindow *ui)
{
    // start export in a diff thread
    QImage img(scene->sceneRect().width(),
               scene->sceneRect().height(),
               QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter);
    painter.end();

    img.save(fileName);
    ui->statusBar->showMessage(tr("MindMap exported as ") + fileName,
                               5000); // millisec
}
*/


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_graphicsView = new GraphWidget(this);

    m_ui->setupUi(this);
    connect(m_ui->actionNew, SIGNAL(activated()),
            m_graphicsView, SLOT(newFile()));
    connect(m_ui->actionOpen, SIGNAL(activated()),
            m_graphicsView, SLOT(openFile()));
    connect(m_ui->actionSave, SIGNAL(activated()),
            m_graphicsView, SLOT(saveFile()));
    connect(m_ui->actionSaveAs, SIGNAL(activated()),
            m_graphicsView, SLOT(saveFileAs()));

    connect(m_ui->actionClose, SIGNAL(activated()),
            m_graphicsView, SLOT(closeFile()));
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

void MainWindow::exportScene()
{
    QFileDialog dialog(this,
                       tr("Export MindMap to image"),
                       "/home/cs0rbagomba",
                       tr("PNG image file (*.png)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("png");


     if (dialog.exec())
     {
         QStringList fileNames(dialog.selectedFiles());

         /// @note Shall I start the export in diff thread?
//         QtConcurrent::run(exportScaneToPng,
//                           graphicsView->getScene(),
//                           fileNames.first(),
//                           ui);

         QImage img(m_graphicsView->getScene()->sceneRect().width(),
                    m_graphicsView->getScene()->sceneRect().height(),
                    QImage::Format_ARGB32_Premultiplied);
         QPainter painter(&img);

         painter.setRenderHint(QPainter::Antialiasing);

         /// @bug scene background is not rendered
         m_graphicsView->getScene()->render(&painter);
         painter.end();

         img.save(fileNames.first());
         m_ui->statusBar->showMessage(tr("MindMap exported as ") + fileNames.first(),
                                    5000); // millisec

     }
}

void MainWindow::about()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("About QtMindMap"));
    msgBox.setText(tr("MindMap software written in Qt."));
    msgBox.setInformativeText(tr("Homepage: https://gitorious.org/qtmindmap\n\nReport bugs to: denes.matetelki@gmail.com"));
    QPixmap pixMap(":/heart.svg");
    msgBox.setIconPixmap(pixMap.scaled(50,50));
    msgBox.exec();
}

QStatusBar * MainWindow::getStatusBar()
{
    return m_ui->statusBar;
}
