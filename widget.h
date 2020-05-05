#ifndef WIDGET_H
#define WIDGET_H
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
namespace Ui { class Widget; }
class Widget : public QWidget {
    Q_OBJECT
    Ui::Widget *ui;
public:
    Widget(QWidget *parent = 0);
    ~Widget();
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

    QVector<QPair<QString, QString>> chartData;

private slots: //слоты для действий, выполняемых по кнопкам
    void insertChild();
    bool insertColumn();
    void insertRow();
    bool removeColumn();
    void removeRow();
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
#endif // WIDGET_H
