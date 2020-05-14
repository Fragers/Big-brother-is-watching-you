#include "initstart.h"
#include "ui_initstart.h"

initStart::initStart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::initStart)
{
    ui->setupUi(this);
}

initStart::~initStart()
{
    delete ui;
}

void initStart::on_OpenF_clicked()
{
    state = "open";
    close();
}

void initStart::on_NewF_clicked()
{

    state = "new";
    close();
}

void initStart::on_CloseF_clicked()
{

    state = "close";
    close();
}
