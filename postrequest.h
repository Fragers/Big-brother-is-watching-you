#ifndef POSTREQUEST_H
#define POSTREQUEST_H

#include <QObject>
#include<QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include<iostream>
#include<QByteArray>
#include<QFile>
#include<QDebug>
#include<QNetworkReply>
class postRequest : public QObject
{
    Q_OBJECT
public:
    explicit postRequest( QObject *parent = nullptr);
    ~postRequest();
    QString curName;
    QString dt;
signals:
    void onReady();

public slots:
    void postData();
    void onResult(QNetworkReply * reply);

private:
    QNetworkAccessManager *manager;
};

#endif // POSTREQUEST_H
