#include "parseitems.h"
#include "ui_parseitems.h"

parseItems::parseItems(QStringList items, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::parseItems)
{
    ui->setupUi(this);
    ui->listWidget->clear();
    ui->listWidget->addItems(items);
}

parseItems::~parseItems()
{
    delete ui;
}

void parseItems::on_listWidget_currentTextChanged(const QString &currentText)
{
    enabled = currentText;
}

void parseItems::on_pushButton_clicked()
{
   close();
}
