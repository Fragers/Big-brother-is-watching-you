#include "widget.h"
#include "treemodel.h"
#include "ui_widget.h"
#include<QtWidgets>
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {

    ui->setupUi(this);

    /*Server*/

    socket = new QTcpSocket(this);
        connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));


    /*Server*/

    curPath = new QLabel;

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


Widget::~Widget()
{
    delete ui;
}


void Widget::openClicked(){
    initModel();
}

void Widget::saveClicked(){

    docToPush = model1->createFile(file1.fileName());
}

void Widget::initModel(){
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

void Widget::insertChild() {
     //Получаем модельный индекс и модель элемента:
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();

    if(model1->getType(index) == "task")
        return;
     //Вставляем данные:
    TreeItem * it = model1->getItem(index);
    index = model1->getIn(0, 0, it);

    //index = it;
    if (model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index)) return;
    }
    if (!model->insertRow(0, index)) return;
     //Инициализируем их:
    for (int column = 0; column < model->columnCount(index); ++column) {

        QModelIndex child = model->index(0, column, index);
        if(model1->getType(index) == "group"){
            model1->setType(child, "employee");
        }
        if(model1->getType(index) == "employee"){
            model1->setType(child, "task");
        }

        QString type = model1->getType(index);

        if(column > 0 ) {
            if(type == "employee")
                model->setData(child, QVariant("Данные"), Qt::EditRole);
            else
                model->setData(child, QVariant(""), Qt::EditRole);
        }else
            model->setData(child, QVariant(model1->getType(child)), Qt::EditRole);

        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant("Столбец"), Qt::EditRole);
      }
     //Выбираем вставленный узел:
     ui->treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
        QItemSelectionModel::ClearAndSelect);
     //Меняем состояние кнопок:
     updateActions();
}

bool Widget::insertColumn() {
 QAbstractItemModel *model = ui->treeView->model();
 int column = ui->treeView->selectionModel()->currentIndex().column();
 bool changed = model->insertColumn(column + 1);
 if (changed)
  model->setHeaderData(column + 1, Qt::Horizontal, QVariant("Столбец"), Qt::EditRole);
 updateActions();
 return changed;
}

void Widget::insertRow() {
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();

    if (!model->insertRow(index.row()+1, index.parent())) return;
            updateActions();

    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        QModelIndex child = model->index(index.row()+1, column, index.parent());
        model1->setType(child, model1->getType(index));
        if(column > 0){
            if(model1->getType(index) == "task")
                model->setData(child, QVariant("Данные"), Qt::EditRole);
            else
                model->setData(child, QVariant(""), Qt::EditRole);
        }
        else
            model->setData(child, QVariant(model1->getType(index)), Qt::EditRole);

    }
}

bool Widget::removeColumn() {
 QAbstractItemModel *model = ui->treeView->model();
 int column = ui->treeView->selectionModel()->currentIndex().column();
 bool changed = model->removeColumn(column); //Удалить столбец для каждого потомка
 if (changed) updateActions();
 return changed;
}

void Widget::removeRow() {
 QModelIndex index = ui->treeView->selectionModel()->currentIndex();
 QAbstractItemModel *model = ui->treeView->model();
 if (model->removeRow(index.row(), index.parent())) updateActions();
}

void Widget::updateActions(const QItemSelection &selected,const QItemSelection &deselected) {
    Q_UNUSED(selected)
    Q_UNUSED(deselected)
    updateActions();
}

void Widget::updateActions() {
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

void Widget::on_install_clicked()
{
    if(socket->isOpen()){
        socket->write("Read");
        socket->waitForBytesWritten(2000);
    }else{
        QMessageBox::information(this, "Иноформация", "Соединение не установлено");
    }


}

//Сервер

void Widget::on_ConnectTo_clicked()
{

    socket->connectToHost("127.0.0.1", 5555);
}

void Widget::sockDisc(){
    socket->deleteLater();
}

void Widget::sockReady(){
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

void Widget::on_pushButton_clicked()
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
