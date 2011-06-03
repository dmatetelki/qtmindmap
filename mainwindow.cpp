#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

#include <QDebug>
//#include <QLayout>
#include <QFileDialog>


MainWindow::MainWindow(bool isSystemtray, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    aboutDialog(0)
{
    qDebug() << __PRETTY_FUNCTION__;
    ui->setupUi(this);
    connect(ui->actionNew, SIGNAL(activated()), this, SLOT(klakk()));
    connect(ui->actionOpen, SIGNAL(activated()), this, SLOT(klakk()));
    connect(ui->actionSave, SIGNAL(activated()), this, SLOT(klakk()));
    connect(ui->actionClose, SIGNAL(activated()), this, SLOT(klakk()));
    connect(ui->actionExport, SIGNAL(activated()), this, SLOT(exportScene()));
    connect(ui->actionQuit, SIGNAL(activated()), QApplication::instance(),
            SLOT(quit()));
    connect(ui->actionAbout_QtMindMap, SIGNAL(activated()), this,
            SLOT(about()));

    graphicsView = new GraphWidget(ui->centralWidget);
    setCentralWidget(graphicsView);

    if (isSystemtray) setupSystemTray();
}

MainWindow::~MainWindow()
{
    qDebug() << __PRETTY_FUNCTION__;
    delete ui;
    if (aboutDialog) delete aboutDialog;
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

         // start export in a diff thread
         QImage img(graphicsView->getScene()->sceneRect().width(),
                    graphicsView->getScene()->sceneRect().height(),
                    QImage::Format_ARGB32_Premultiplied);
         QPainter painter(&img);
         painter.setRenderHint(QPainter::Antialiasing);
         graphicsView->getScene()->render(&painter);
         painter.end();

         img.save(fileNames.first());
         ui->statusBar->showMessage(tr("MindMap exported as ") + fileNames.first(),
                                    5000);
     }
}

void MainWindow::setupSystemTray()
{
    systemTrayIcon = new QSystemTrayIcon(0);

    minimizeAction = new QAction(tr("Mi&nimize"), systemTrayIcon);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), systemTrayIcon);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), systemTrayIcon);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), systemTrayIcon);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    systemTrayIcon->setContextMenu(trayIconMenu);

    icon = new QIcon(":/heart.svg");
    systemTrayIcon->setIcon(QIcon(":/heart.svg"));
}

void MainWindow::about()
{
    qDebug() << __PRETTY_FUNCTION__;

    setDisabled(true);
    if (aboutDialog == 0) aboutDialog = new AboutDialog(this);
    aboutDialog->setEnabled(true); // children inherits enabled status
    aboutDialog->show();
//    aboutDialog->layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void MainWindow::aboutDestroyed()
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << aboutDialog;
    setEnabled(true);

}

void MainWindow::showSysTray()
{
    systemTrayIcon->show();
}
