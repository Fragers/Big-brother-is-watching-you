#include "widget.h"
#include "mainwindow.h"
#include <QApplication>
#include"myserver.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
  //  myserver Server;
  //  Server.startServer();
    MainWindow w;
    w.show();
    return a.exec();
}
