#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

#include <QDebug>
#include <QFileDialog>

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
    m_ui(new Ui::MainWindow),
    m_aboutDialog(0)
{
    qDebug() << __PRETTY_FUNCTION__;
    m_ui->setupUi(this);
    connect(m_ui->actionNew, SIGNAL(activated()), this, SLOT(klakk()));
    connect(m_ui->actionOpen, SIGNAL(activated()), this, SLOT(klakk()));
    connect(m_ui->actionSave, SIGNAL(activated()), this, SLOT(klakk()));
    connect(m_ui->actionClose, SIGNAL(activated()), this, SLOT(klakk()));
    connect(m_ui->actionExport, SIGNAL(activated()), this, SLOT(exportScene()));
    connect(m_ui->actionQuit, SIGNAL(activated()), QApplication::instance(),
            SLOT(quit()));
    connect(m_ui->actionAbout_QtMindMap, SIGNAL(activated()), this,
            SLOT(about()));

    m_graphicsView = new GraphWidget(this);
    setCentralWidget(m_graphicsView);
}

MainWindow::~MainWindow()
{
    qDebug() << __PRETTY_FUNCTION__;
    delete m_ui;
    if (m_aboutDialog) delete m_aboutDialog;
}

void MainWindow::klakk()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void MainWindow::exportScene()
{
    qDebug() << __PRETTY_FUNCTION__;

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
    qDebug() << __PRETTY_FUNCTION__;

    setDisabled(true);
    if (m_aboutDialog == 0) m_aboutDialog = new AboutDialog(this);
    m_aboutDialog->setEnabled(true); // children inherits enabled status
    m_aboutDialog->show();
}

void MainWindow::aboutDestroyed()
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << m_aboutDialog;
    setEnabled(true);

}

QStatusBar * MainWindow::getStatusBar()
{
    return m_ui->statusBar;
}
