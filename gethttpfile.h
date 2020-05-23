#ifndef GETHTTPFILE_H
#define GETHTTPFILE_H

#include <QObject>
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QFile>
#include<QUrl>
#include<QDebug>
#include<QStringList>
#include<QString>

class getHttpFile : public QObject
{
    Q_OBJECT
public:
    explicit getHttpFile( QObject *parent = nullptr);
    QStringList list;
    QStringList newList;
    QString curFile;

signals:
    void onReady();

public slots:
    void getData();
    void onResult(QNetworkReply * reply);

private:
    QNetworkAccessManager *manager;
};
#endif // GETHTTPFILE_H
