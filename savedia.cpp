#include "savedia.h"
#include "ui_savedia.h"

saveDia::saveDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::saveDia)
{
    ui->setupUi(this);
}

saveDia::~saveDia()
{
    delete ui;
}

void saveDia::on_pushButton_clicked()
{
    redj = true;
    acc = true;
    close();
}

void saveDia::on_pushButton_2_clicked()
{
    redj = false;
    acc = true;
    close();
}
