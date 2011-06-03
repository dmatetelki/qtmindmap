#include "aboutdialog.h"
#include "ui_aboutdialog.h"


AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    connect(this, SIGNAL(finished(int)), parent,
            SLOT(aboutDestroyed()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
