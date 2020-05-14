#include "mainwindow.h"
#include "treemodel.h"
#include "ui_mainwindow.h"
#include<QtWidgets>
#include <QAction>
#include"chart.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtCharts/QChartView>
#include "donutbreakdownchart.h"
#include"savedia.h"
#include<QLayout>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    /*Server*/

    socket = new QTcpSocket(this);
        connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));

        connect(ui->actionLoad_file, SIGNAL(triggered()), this, SLOT(loadTriggered()));
        connect(ui->actionUpload_file, SIGNAL(triggered()), this, SLOT(loadTriggered()));
        connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(connectTriggered()));

    /*Server*/

    curPath = new QLabel;
    ui->statusbar->addWidget(curPath);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(saveClicked()));

    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(openClicked()));

    connect(ui->action_Exit, SIGNAL(triggered()), this, SLOT(exitApp()));

    connect(ui->actionNew_file, SIGNAL(triggered()), this, SLOT(newFile()));

    Q_INIT_RESOURCE(simpletreemodel);
    initModel();



    connect(ui->insertRowAction,SIGNAL(clicked()),this,SLOT(insertRow()));


    /*columns*/
//    connect(ui->insertColumnAction,SIGNAL(clicked()),this,SLOT(insertColumn()));

//    connect(ui->removeColumnAction, SIGNAL(clicked()),this,SLOT(removeColumn()));

    connect(ui->removeRowAction,SIGNAL(clicked()),this,SLOT(removeRow()));


    connect(ui->insertChildAction, SIGNAL(clicked()),this,SLOT(insertChild()));

    connect(ui->actionShow_chart, SIGNAL(triggered()), this, SLOT(showChart()));


    /*ShortCuts*/
    ui->action_Save->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    ui->action_Open->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    ui->action_Exit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    ui->actionNew_file->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));

    /*ShortCuts*/



     //и обновить состояние кнопок:
    updateActions();
}

void MainWindow::checkSaveDia(){

    QDialog* dia = new QDialog(this);
    QHBoxLayout *lay = new QHBoxLayout();
    QPushButton *yes1 = new QPushButton();

    QPushButton *no1 = new QPushButton();
    dia->setWindowTitle("Warning");
    yes1->setText("Сохранить");
    no1->setText("Не сохранять");

    QLabel *label = new QLabel();
    label->setText("Вы не сохранили текущий документ.");
    label->setAlignment(Qt::AlignCenter);
    connect(yes1, SIGNAL(clicked()), dia, SLOT(accept()));

    connect(no1, SIGNAL(clicked()), dia, SLOT(reject()));
    lay->addWidget(yes1);
    lay->addWidget(no1);
    QVBoxLayout *hbox = new QVBoxLayout();
    hbox->addWidget(label);
    hbox->addItem(lay);

    dia->setLayout(hbox);

    if(dia->exec() == QDialog::Accepted){
        saveClicked();
    }

}

void MainWindow::newFile(){

    if(!isSaved){
        checkSaveDia();
    }

     file1.setFileName(QFileDialog::getSaveFileName());
     if(file1.open(QIODevice::WriteOnly|QFile::Text)){
        file1.close();
        flagGetFile = 1;
        initModel();
     }


}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isSaved == false){
        event->ignore();
        exitApp();
    }
}

void MainWindow::exitApp(){
    if(isSaved == true)
        close();

    saveDia *dia = new saveDia(this);

    dia->exec();

    if(dia->state == "save"){
        docToPush = model1->createFile(file1.fileName());
        isSaved = true;
        exit(0);
    }

    if(dia->state == "cancel"){

    }
    if(dia->state == "close"){
        exit(0);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showChart(){

    chartData.clear();
    TreeItem* root = model1->getRoot();
    int chCount = root->childCount();

    for(int i = 0; i < chCount; i++){
        TreeItem* cur =  root->child(i);
        QString per = cur->data(2).toString();
        QString grName = cur->data(0).toString();
        per.resize(per.size()-1);
        double dPer = per.toDouble();
        chartData.push_back(qMakePair(grName, dPer));

    }

    window = new chart(this, chartData);
    window->show();

}

void MainWindow::openClicked(){

    if(!isSaved){
        checkSaveDia();
    }

    initModel();

}

void MainWindow::saveClicked(){

    docToPush = model1->createFile(file1.fileName());
    isSaved = true;
}

void MainWindow::initModel(){
    if(flagGetFile == 0)
        file1.setFileName(QFileDialog::getOpenFileName(nullptr, "", "./..", "*.json"));

    if(file1.open(QIODevice::ReadOnly|QFile::Text)){
        docToPush = file1.readAll();
        doc = QJsonDocument::fromJson(QByteArray(docToPush), &docError);
        file1.close();
    }else{
        if(!hasData)
            exit(0);
        else {
            return;
        }
    }

    QJsonArray docAr = QJsonValue(doc.object().value("groups")).toArray();

    QStringList headers;

    headers <<  "Отчет1" << "выполнени1е" << "срок1";

    //Загружаем данные в модель:
    //delete model1;
    TreeModel *model = new TreeModel(headers, doc);

    model1 = model;

    int id = QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-SemiBold.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont monospace(family);

    ui->treeView->setFont(monospace);
    ui->treeView->setModel(model);
    //QFont font(":/MODERNE_SANS.ttf");
    connect(ui->treeView->selectionModel(),SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
             this, SLOT(updateActions(const QItemSelection&,const QItemSelection&)));

    for (int column = 0; column < model->columnCount(); ++column)
        ui->treeView->resizeColumnToContents(column);

    curPath->setText(file1.fileName());

    flagGetFile = 0;
    updateActions();
    newUpdate = 1;
    hasData = 1;
    isSaved = false;
}

void MainWindow::insertChild() {
     //Получаем модельный индекс и модель элемента:
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();

    if(model1->getType(index) == "task")
        return;
     //Вставляем данные:
    //QModelIndex parInd = model1->parent(index);
    TreeItem * parIt = static_cast<TreeItem*>(index.internalPointer());


    index = model1->getIn(index.row(), 0, parIt);

    //index = it;
    if (model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index)) return;
    }
    if (!model->insertRow(0, index)) return;
     //Инициализируем их:
    for (int column = 0; column < model->columnCount(index); ++column) {

        QModelIndex child = model->index(0, column, index);

        if(model1->getType(index) == "root"){
            model1->setType(child, "group");
        }
        if(model1->getType(index) == "group"){
            model1->setType(child, "employee");
        }
        if(model1->getType(index) == "employee"){
            model1->setType(child, "task");
        }

        QString type = model1->getType(index);

        if(column > 0 ) {
            if(type == "employee"){
                if(column == 2)
                    model->setData(child, QVariant("in progress"), Qt::EditRole);
                else
                    model->setData(child, QVariant("Данные"), Qt::EditRole);
            }
            else
                model->setData(child, QVariant(""), Qt::EditRole);
        }else
            model->setData(child, QVariant(QString(model1->getType(child) +
                                                   QString::number(parIt->childCount()))), Qt::EditRole);

        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant("Столбец"), Qt::EditRole);
      }
     //Выбираем вставленный узел:
     ui->treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
        QItemSelectionModel::ClearAndSelect);
     //Меняем состояние кнопок:
     isSaved = false;
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
    TreeItem* parIt = model1->getItem(index.parent());
    if (!model->insertRow(index.row()+1, index.parent())) return;
            // updateActions();

    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        QModelIndex child = model->index(index.row()+1, column, index.parent());
        model1->setType(child, model1->getType(index));
        if(column > 0){
            if(model1->getType(index) == "task"){
                if(column == 2)
                    model->setData(child, QVariant("in progress"), Qt::EditRole);
                else
                    model->setData(child, QVariant("Данные"), Qt::EditRole);
            }
            else
                model->setData(child, QVariant(""), Qt::EditRole);
        }
        else
            model->setData(child, QVariant(QString(model1->getType(child) + QString::number(parIt->childCount()))), Qt::EditRole);

    }
    isSaved = false;
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
 isSaved = false;
}

void MainWindow::updateActions(const QItemSelection &selected,const QItemSelection &deselected) {
    Q_UNUSED(selected)
    Q_UNUSED(deselected)
    updateActions();
}

void MainWindow::updateActions() {
 //Обновим состояние кнопок:
    bool hasSelection = !ui->treeView->selectionModel()->selection().isEmpty();

    ui->removeRowAction->setEnabled(hasSelection);

    //ui->removeColumnAction->setEnabled(hasSelection);

    bool hasCurrent = ui->treeView->selectionModel()->currentIndex().isValid();
    ui->insertRowAction->setEnabled(hasCurrent);
         //Покажем информацию в заголовке окна:
    if (hasCurrent && (newUpdate == 0)) {
        ui->treeView->closePersistentEditor(ui->treeView->selectionModel()->currentIndex());

        int row = ui->treeView->selectionModel()->currentIndex().row();

        int column = ui->treeView->selectionModel()->currentIndex().column();
        QModelIndex cur = ui->treeView->selectionModel()->currentIndex();
        TreeItem* curItem = model1->getItem(cur);
        QString type = model1->getType(cur);
        QModelIndex pInd = model1->parent(cur);
        TreeItem* parIt = model1->getItem(pInd);
        QString parName = parIt->data(0).toString();
        ///////////
        if (ui->treeView->selectionModel()->currentIndex().parent().isValid())
         this->setWindowTitle(tr("(row,col)=(%1,%2)").arg(row).arg(column));
        else
         this->setWindowTitle(tr("(row,col)=(%1,%2) ВЕРХ").arg(row).arg(column));
        ///////////
        if(type == "task"){
            ui->insertRowAction->setText(QString("Добавить задачу для\n %1").arg(parIt->data(0).toString()));
            ui->insertChildAction->setText("");
            ui->removeRowAction->setText(QString("Удалить задачу\n %1").arg(curItem->data(0).toString()));
        }
        if(type == "group"){
            ui->insertRowAction->setText(QString("Добавить новую группу"));
            ui->insertChildAction->setText(QString("Добавить работника\nв группу\n %1").arg(curItem->data(0).toString()));
            ui->removeRowAction->setText(QString("Удалить группу\n %1").arg(curItem->data(0).toString()));

        }
        if(type == "employee"){
            ui->insertRowAction->setText(QString("Добавить нового\nсотрудника в группу\n%1").arg(parName));
            ui->insertChildAction->setText(QString("Добавить задание для\n %1").arg(curItem->data(0).toString()));
            ui->removeRowAction->setText(QString("Удалить работника\n %1").arg(curItem->data(0).toString()));

        }
    }else{

        if(model1->getRoot()->childCount()){
            TreeItem* root = model1->getRoot();
            TreeItem* first = root->child(0);
            QModelIndex ind = model1->getIn(0, 0, first);

            ui->treeView->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select);
            ui->treeView->closePersistentEditor(ui->treeView->selectionModel()->currentIndex());
            connect(model1, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                           this, SLOT(updateActions2()));
        }else{
            setNewGroup();

            TreeItem* root = model1->getRoot();
            TreeItem* first = root->child(0);
            QModelIndex ind = model1->getIn(0, 0, first);

            ui->treeView->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select);
            ui->treeView->closePersistentEditor(ui->treeView->selectionModel()->currentIndex());

        }
    }
    if(newUpdate == 1){
        newUpdate = 0;
        connect(model1, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
              this, SLOT(updateActions2()));
    }

}

void MainWindow::updateActions2(){
    isSaved = false;
    updateActions();
}
void MainWindow::setNewGroup(){

   TreeItem* root = model1->getRoot();
   QModelIndex index = model1->getIn(0, 0, root);
   QAbstractItemModel *model = ui->treeView->model();

   TreeItem * parIt = static_cast<TreeItem*>(index.internalPointer());


   index = model1->getIn(0, 0, parIt);

   //index = it;
   if (model->columnCount(index) == 0) {
       if (!model->insertColumn(0, index)) return;
   }
   if (!model->insertRow(0, index)) return;
    //Инициализируем их:
   for (int column = 0; column < model->columnCount(index); ++column) {

       QModelIndex child = model->index(0, column, index);

       model1->setType(child, "group");

       QString type = model1->getType(index);

       if(column > 0 ) {
               model->setData(child, QVariant(""), Qt::EditRole);
       }else
           model->setData(child, QVariant(model1->getType(child)), Qt::EditRole);

       if (!model->headerData(column, Qt::Horizontal).isValid())
           model->setHeaderData(column, Qt::Horizontal, QVariant("Столбец"), Qt::EditRole);
     }
    //Выбираем вставленный узел:
    ui->treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
       QItemSelectionModel::ClearAndSelect);
}


void MainWindow::loadTriggered()
{
    if(socket->isOpen()){
        socket->write("Read");
        socket->waitForBytesWritten(2000);
    }else{
        QMessageBox::information(this, "Иноформация", "Соединение не установлено");
    }


}



void MainWindow::connectTriggered()
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

void MainWindow::uploadTriggered()
{
    if(socket->isOpen()){
        socket->write("Write");
        socket->waitForBytesWritten(1000);
        socket->write(docToPush);
    }else{
        QMessageBox::information(this, "Иноформация", "Соединение не установлено");
    }

}

//Сервер

//void MainWindow::on_install_clicked()
//{
//    if(socket->isOpen()){
//        socket->write("Read");
//        socket->waitForBytesWritten(2000);
//    }else{
//        QMessageBox::information(this, "Иноформация", "Соединение не установлено");
//    }


//}



//void MainWindow::on_ConnectTo_clicked()
//{

//    socket->connectToHost("127.0.0.1", 5555);
//}

//void MainWindow::sockDisc(){
//    socket->deleteLater();
//}

//void MainWindow::sockReady(){
//    if(socket->waitForConnected(500)){
//        socket->waitForReadyRead(500);
//        Data = socket->readAll();
//        testDoc = QJsonDocument::fromJson(Data, &testDocError);
//        if(testDocError.errorString().toInt() == QJsonParseError::NoError){
//            if(testDoc.object().value("type").toString() == "connect" && testDoc.object().value("status").toString() == "yes"){
//                QMessageBox::information(this, "информация", "соединение установлено");

//            }else{
//                flagGetFile = 1;
//                qDebug() << Data;
//                if(file1.open(QIODevice::WriteOnly|QIODevice::Text)){
//                    file1.write(testDoc.toJson());
//                }
//                file1.close();
//                initModel();

//            }
//        }else
//            QMessageBox::information(this, "информация", "соединение не установлено");
//        qDebug() << Data;
//    }
//}

//void MainWindow::on_pushButton_clicked()
//{
//    if(socket->isOpen()){
//        socket->write("Write");
//        socket->waitForBytesWritten(1000);
//        socket->write(docToPush);
//    }else{
//        QMessageBox::information(this, "Иноформация", "Соединение не установлено");
//    }

//}

//todo кидаем запрос на чтение в формате
