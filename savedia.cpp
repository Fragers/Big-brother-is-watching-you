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


void saveDia::on_Cancel_clicked()
{
    state = "cancel";
    close();
}

void saveDia::on_Close_clicked()
{

    state = "close";
    close();
}

void saveDia::on_SaveAll_clicked()
{
    state = "save";
    close();
}
