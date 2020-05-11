#include "createnewfile.h"
#include "ui_createnewfile.h"
#include <QFile>
#include <QFileDialog>
CreateNewFile::CreateNewFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNewFile)
{
   ui->setupUi(this);
}

CreateNewFile::~CreateNewFile()
{
    delete ui;
}

void CreateNewFile::on_Cancel_clicked()
{
    close();
}

void CreateNewFile::on_Create_clicked()
{

}
