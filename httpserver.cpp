#include "httpserver.h"
#include<QRegExp>
#include<string>
httpServer::httpServer(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished, this, &httpServer::onResult);
}

void httpServer::getData(){
    QUrl url("http://kappa.cs.petrsu.ru/~simagin/web/");
    QNetworkRequest request;
    request.setUrl(url);
    manager->get(request);
}

void httpServer::onResult(QNetworkReply *reply){
    if(reply->error()){
        qDebug() << "Error";
        qDebug() << reply->errorString();
    } else {
        newList.clear();
        QRegExp re(".*<a href=\".*.json\">.*");
        QString str1 = reply->readAll();
        list = str1.split("\n");
        foreach(QString str, list){
            if (re.exactMatch( str )){
                std::string s = str.toStdString();
                s = s.substr(52);
                int n = s.find(".json");
                s = s.substr(0, n+5);
                QString se = QString::fromStdString(s);
                newList.append(se);
                //qDebug() << se;
            }
        }
        reply->deleteLater();
        emit onReady();
    }
}
