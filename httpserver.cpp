#include "httpserver.h"

httpServer::httpServer(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished, this, &httpServer::onResult);
}

void httpServer::getData(){
    QUrl url("http://kappa.cs.petrsu.ru/~simagin/123.json");
    QNetworkRequest request;
    request.setUrl(url);
    manager->get(request);
}

void httpServer::onResult(QNetworkReply *reply){
    if(reply->error()){
        qDebug() << "Error";
        qDebug() << reply->errorString();
    } else {
        QFile file("E://qt_creator//projects//123//123//webFiles//testFile.json");
        if(file.open(QFile::WriteOnly)){
            file.write(reply->readAll());
            file.close();
        }
        qDebug() << "successfully downloaded";
        emit onReady();
    }

}
