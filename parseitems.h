#ifndef PARSEITEMS_H
#define PARSEITEMS_H

#include <QDialog>
#include<QStringList>
#include<QString>
#include <QObject>
#include<QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include<iostream>
#include<QByteArray>
#include<QFile>
#include<QDebug>
#include<QNetworkReply>
namespace Ui {
class parseItems;
}

class parseItems : public QDialog
{
    Q_OBJECT

public:
    explicit parseItems(QStringList items, QWidget *parent = nullptr);
    ~parseItems();
    QString enabled;
   // QStringList items;
private slots:
    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_pushButton_clicked();

private:
    Ui::parseItems *ui;

};

#endif // PARSEITEMS_H
