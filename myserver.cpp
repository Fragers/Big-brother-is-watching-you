#include"myserver.h"

myserver::myserver(){}

myserver::~myserver(){};

void myserver::startServer(){
    if(this->listen(QHostAddress::Any, 5555)){
        qDebug() << "listening";

    }else
        qDebug() << "Not listening";
}

void myserver::incomingConnection(qintptr socketDescriptor){
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));

    qDebug() << socketDescriptor<<" connected";

    socket->write("You are connected");

    qDebug() << "Send cliend connect status - YES";


}

void myserver::sockReady(){


}

void myserver::sockDisc(){
    qDebug() << "Disconnect";

    socket->deleteLater();
}
