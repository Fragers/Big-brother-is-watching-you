#include "mainwindow.h"
#include "treemodel.h"
#include "ui_mainwindow.h"
#include<QtWidgets>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    /*Server*/

    socket = new QTcpSocket(this);
        connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));


    /*Server*/

    curPath = new QLabel;
    ui->statusbar->addWidget(curPath);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(saveClicked()));

    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(openClicked()));
    Q_INIT_RESOURCE(simpletreemodel);
    initModel();

    connect(ui->treeView->selectionModel(),SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
             this, SLOT(updateActions(const QItemSelection&,const QItemSelection&)));

    connect(ui->insertRowAction,SIGNAL(clicked()),this,SLOT(insertRow()));

    connect(ui->insertColumnAction,SIGNAL(clicked()),this,SLOT(insertColumn()));

    connect(ui->removeRowAction,SIGNAL(clicked()),this,SLOT(removeRow()));

    connect(ui->removeColumnAction,SIGNAL(clicked()),this,SLOT(removeColumn()));

    connect(ui->insertChildAction,SIGNAL(clicked()),this,SLOT(insertChild()));
     //и обновить состояние кнопок:
updateActions();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openClicked(){
    initModel();
}

void MainWindow::saveClicked(){

    docToPush = model1->createFile(file1.fileName());
}

void MainWindow::initModel(){
    if(flagGetFile == 0)
        file1.setFileName(QFileDialog::getOpenFileName(nullptr, "", ".", "*.json"));

    if(file1.open(QIODevice::ReadOnly|QFile::Text)){
        //qDebug() << 1;
        docToPush = file1.readAll();
        doc = QJsonDocument::fromJson(QByteArray(docToPush), &docError);
        file1.close();
    }

    QJsonArray docAr = QJsonValue(doc.object().value("groups")).toArray();

    QStringList headers;

    headers <<  "Отчет1" << "выполнени1е" << "срок1";

    //Загружаем данные в модель:
    //delete model1;
    TreeModel *model = new TreeModel(headers, doc);

    model1 = model;
   // file.close();

    ui->treeView->setModel(model);

    for (int column = 0; column < model->columnCount(); ++column)
        ui->treeView->resizeColumnToContents(column);
    curPath->setText(file1.fileName());

    flagGetFile = 0;
}

void MainWindow::insertChild() {
 //Получаем модельный индекс и модель элемента:
 QModelIndex index = ui->treeView->selectionModel()->currentIndex();
 QAbstractItemModel *model = ui->treeView->model();
 //Вставляем данные:
 if (model->columnCount(index) == 0) {
  if (!model->insertColumn(0, index)) return;
 }
 if (!model->insertRow(0, index)) return;
 //Инициализируем их:
 for (int column = 0; column < model->columnCount(index); ++column) {
  QModelIndex child = model->index(0, column, index);
  model->setData(child, QVariant("Данные"), Qt::EditRole);
  if (!model->headerData(column, Qt::Horizontal).isValid())
   model->setHeaderData(column, Qt::Horizontal, QVariant("Столбец"), Qt::EditRole);
  }
 //Выбираем вставленный узел:
 ui->treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
  QItemSelectionModel::ClearAndSelect);
 //Меняем состояние кнопок:
 updateActions();
}

bool MainWindow::insertColumn() {
 QAbstractItemModel *model = ui->treeView->model();
 int column = ui->treeView->selectionModel()->currentIndex().column();
 bool changed = model->insertColumn(column + 1);
 if (changed)
  model->setHeaderData(column + 1, Qt::Horizontal, QVariant("Столбец"), Qt::EditRole);
 updateActions();
 return changed;
}

void MainWindow::insertRow() {
     QModelIndex index = ui->treeView->selectionModel()->currentIndex();
     QAbstractItemModel *model = ui->treeView->model();

     if (!model->insertRow(index.row()+1, index.parent())) return;
        updateActions();

     for (int column = 0; column < model->columnCount(index.parent()); ++column) {
          QModelIndex child = model->index(index.row()+1, column, index.parent());
          model->setData(child, QVariant("Данные"), Qt::EditRole);
      }
}

bool MainWindow::removeColumn() {
 QAbstractItemModel *model = ui->treeView->model();
 int column = ui->treeView->selectionModel()->currentIndex().column();
 bool changed = model->removeColumn(column); //Удалить столбец для каждого потомка
 if (changed) updateActions();
 return changed;
}

void MainWindow::removeRow() {
 QModelIndex index = ui->treeView->selectionModel()->currentIndex();
 QAbstractItemModel *model = ui->treeView->model();
 if (model->removeRow(index.row(), index.parent())) updateActions();
}

void MainWindow::updateActions(const QItemSelection &selected,const QItemSelection &deselected) {
    updateActions();
}

void MainWindow::updateActions() {
 //Обновим состояние кнопок:
     bool hasSelection = !ui->treeView->selectionModel()->selection().isEmpty();

     ui->removeRowAction->setEnabled(hasSelection);

     ui->removeColumnAction->setEnabled(hasSelection);

     bool hasCurrent = ui->treeView->selectionModel()->currentIndex().isValid();
     ui->insertRowAction->setEnabled(hasCurrent);
     ui->insertColumnAction->setEnabled(hasCurrent);
     //Покажем информацию в заголовке окна:
     if (hasCurrent) {
          ui->treeView->closePersistentEditor(ui->treeView->selectionModel()->currentIndex());

          int row = ui->treeView->selectionModel()->currentIndex().row();

          int column = ui->treeView->selectionModel()->currentIndex().column();

          if (ui->treeView->selectionModel()->currentIndex().parent().isValid())
             this->setWindowTitle(tr("(row,col)=(%1,%2)").arg(row).arg(column));
          else
             this->setWindowTitle(tr("(row,col)=(%1,%2) ВЕРХ").arg(row).arg(column));
         }
}

void MainWindow::on_install_clicked()
{
    if(socket->isOpen()){
        socket->write("Read");
        socket->waitForBytesWritten(2000);
    }else{
        QMessageBox::information(this, "Иноформация", "Соединение не установлено");
    }


}

//Сервер

void MainWindow::on_ConnectTo_clicked()
{

    socket->connectToHost("127.0.0.1", 5555);
}

void MainWindow::sockDisc(){
    socket->deleteLater();
}

void MainWindow::sockReady(){
    if(socket->waitForConnected(500)){
        socket->waitForReadyRead(500);
        Data = socket->readAll();
        testDoc = QJsonDocument::fromJson(Data, &testDocError);
        if(testDocError.errorString().toInt() == QJsonParseError::NoError){
            if(testDoc.object().value("type").toString() == "connect" && testDoc.object().value("status").toString() == "yes"){
                QMessageBox::information(this, "информация", "соединение установлено");

            }else{
                flagGetFile = 1;
                qDebug() << Data;
                if(file1.open(QIODevice::WriteOnly|QIODevice::Text)){
                    file1.write(testDoc.toJson());
                }
                file1.close();
                initModel();

            }



        }else
            QMessageBox::information(this, "информация", "соединение не установлено");
        qDebug() << Data;
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(socket->isOpen()){
        socket->write("Write");
        socket->waitForBytesWritten(1000);
        socket->write(docToPush);
    }else{
        QMessageBox::information(this, "Иноформация", "Соединение не установлено");
    }

}
//todo кидаем запрос на чтение в формате
