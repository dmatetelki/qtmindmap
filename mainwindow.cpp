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


MainWindow::MainWindow(bool isSystemtray, QWidget *parent) :
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

    m_graphicsView = new GraphWidget(m_ui->centralWidget);
    setCentralWidget(m_graphicsView);

    if (isSystemtray) setupSystemTray();
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

void MainWindow::setupSystemTray()
{
    m_systemTrayIcon = new QSystemTrayIcon(0);

    m_minimizeAction = new QAction(tr("Mi&nimize"), m_systemTrayIcon);
    connect(m_minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    m_maximizeAction = new QAction(tr("Ma&ximize"), m_systemTrayIcon);
    connect(m_maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    m_restoreAction = new QAction(tr("&Restore"), m_systemTrayIcon);
    connect(m_restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    m_quitAction = new QAction(tr("&Quit"), m_systemTrayIcon);
    connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_minimizeAction);
    m_trayIconMenu->addAction(m_maximizeAction);
    m_trayIconMenu->addAction(m_restoreAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);

    m_systemTrayIcon->setContextMenu(m_trayIconMenu);

    m_icon = new QIcon(":/heart.svg");
    m_systemTrayIcon->setIcon(QIcon(":/heart.svg"));
}

void MainWindow::about()
{
    qDebug() << __PRETTY_FUNCTION__;

    setDisabled(true);
    if (m_aboutDialog == 0) m_aboutDialog = new AboutDialog(this);
    m_aboutDialog->setEnabled(true); // children inherits enabled status
    m_aboutDialog->show();
//    aboutDialog->layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void MainWindow::aboutDestroyed()
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << m_aboutDialog;
    setEnabled(true);

}

void MainWindow::showSysTray()
{
    m_systemTrayIcon->show();
}
