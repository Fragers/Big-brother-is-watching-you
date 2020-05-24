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
#include"httpserver.h"
#include"postrequest.h"
#include"gethttpfile.h"
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
    void setNewGroup();

    void initStartFunc();
    postRequest* postRequester;
    getHttpFile* getter;
private:
    Ui::MainWindow *ui;
    chart *window;
    int hasData = 0;
    bool isSaved = false;
    void closeEvent(QCloseEvent *event);
    int newUpdate = 0;
    httpServer *httpServ;
private slots: //слоты для действий, выполняемых по кнопкам
    void insertChild();
    bool insertColumn();
    void insertRow();
    bool removeColumn();
    void removeRow();
    void showChart();
    void openClicked();
    void saveClicked();
    void loadTriggered();

    //void uploadTriggered();

    //void connectTriggered();

    void exitApp();

    void newFile();
    void checkSaveDia();
    void saveAs();
public slots: //для реализации сигнала selectionChanged у QTreeView::selectionModel
    void updateActions(const QItemSelection &,const QItemSelection &);
    void updateActions2();
    //void sockReady();
    //void sockDisc();
    void getFileHttp();
    void uploadFileHttp();
    void load();
//    void on_install_clicked();

//    void on_ConnectTo_clicked();

//    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
