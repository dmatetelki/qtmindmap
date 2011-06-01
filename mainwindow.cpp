#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

#include <QDebug>
//#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
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
    connect(ui->actionQuit, SIGNAL(activated()), QApplication::instance(),
            SLOT(closeAllWindows()));
    connect(ui->actionAbout_QtMindMap, SIGNAL(activated()), this,
            SLOT(about()));

    graphicsView = new GraphWidget(ui->centralWidget);
    setCentralWidget(graphicsView);

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


