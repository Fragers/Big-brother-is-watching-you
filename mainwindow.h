#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets>
#include <QFile>
#include <QDir>
#include<QTextCodec>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonParseError>
#include<QFileDialog>
#include<QStandardItemModel>
#include<QFile>
#include<QJsonDocument>
#include"treemodel.h"
#include<QTcpSocket>
#include<QMessageBox>
#include "chart.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateActions(); //слот для обновления состояния кнопок
    QFile file1;
    QJsonDocument doc;
    QJsonParseError docError;
    TreeModel *model1;
    void initModel();
    QLabel *curPath;

    /*Сервер*/
    QTcpSocket* socket;
    QByteArray Data;
    QJsonDocument testDoc;
    QJsonParseError testDocError;
    QByteArray docToPush;
    int flagGetFile = 0;

    QVector<QPair<QString, double>> chartData;
private:
    Ui::MainWindow *ui;
    chart *window;
private slots: //слоты для действий, выполняемых по кнопкам
    void insertChild();
    bool insertColumn();
    void insertRow();
    bool removeColumn();
    void removeRow();
    void showChart();
    void openClicked();
    void saveClicked();
    void on_install_clicked();

    void on_ConnectTo_clicked();

    void on_pushButton_clicked();


public slots: //для реализации сигнала selectionChanged у QTreeView::selectionModel
    void updateActions(const QItemSelection &,const QItemSelection &);
    void sockReady();
    void sockDisc();
};

#endif // MAINWINDOW_H
