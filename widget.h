#ifndef WIDGET_H
#define WIDGET_H
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
namespace Ui { class Widget; }
class Widget : public QWidget {
    Q_OBJECT
    Ui::Widget *ui;
public:
    Widget(QWidget *parent = 0);
    void updateActions(); //слот для обновления состояния кнопок
    QFile file1;
    QJsonDocument doc;
    QJsonParseError docError;
    TreeModel *model1;
    void initModel();
private slots: //слоты для действий, выполняемых по кнопкам
    void insertChild();
    bool insertColumn();
    void insertRow();
    bool removeColumn();
    void removeRow();

    void on_install_clicked();

public slots: //для реализации сигнала selectionChanged у QTreeView::selectionModel
    void updateActions(const QItemSelection &,const QItemSelection &);
};
#endif // WIDGET_H
