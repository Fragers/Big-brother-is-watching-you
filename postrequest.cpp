#include "postrequest.h"

postRequest::postRequest( QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished, this, &postRequest::onResult);
}

postRequest::~postRequest(){
    delete manager;
};

void postRequest::postData(){
    QUrl url("http://kappa.cs.petrsu.ru/~simagin/test.php");
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");
    QByteArray postData;
    postData.append("name="+curName.toUtf8());
    //qDebug() << curName;
    postData.append("&jsonDoc="+dt.toUtf8());
    manager->post(request, postData);
}

void postRequest::onResult(QNetworkReply *reply){
    if(reply->error()){
        qDebug() << "Error";
        qDebug() << reply->errorString();
    } else {
        qDebug() << "successfully uploaded";
        reply->deleteLater();
        emit onReady();
    }

}
