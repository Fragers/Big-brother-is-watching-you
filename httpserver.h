#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QFile>
#include<QUrl>
#include<QDebug>
#include<QStringList>
#include<QString>

class httpServer : public QObject
{
    Q_OBJECT
public:
    explicit httpServer(QObject *parent = nullptr);
    QStringList list;
    QStringList newList;

signals:
    void onReady();

public slots:
    void getData();
    void onResult(QNetworkReply * reply);

private:
    QNetworkAccessManager *manager;
};

#endif // HTTPSERVER_H
