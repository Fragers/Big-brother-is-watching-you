#ifndef MYSERVER_H
#define MYSERVER_H

#include<QTcpServer>
#include<QTcpSocket>

class myserver: public QTcpServer
{
    Q_OBJECT
public:
    myserver();
    ~myserver();

    QTcpSocket* socket;
    QByteArray Data;

public slots:
    void startServer();
    void incomingConnection(qintptr socketDiscriptor);
    void sockReady();
    void sockDisc();
};

#endif // MYSERVER_H
