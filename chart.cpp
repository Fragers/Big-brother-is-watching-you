#include "chart.h"
#include "ui_chart.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtCharts/QChartView>
#include "donutbreakdownchart.h"
#include <QDebug>
#include <QDateTime>
chart::chart(QWidget *parent, QVector<QPair<QString, double>> chartData) :
    QDialog(parent),
    ui(new Ui::chart)
{
    ui->setupUi(this);
    data = chartData;
    qDebug() << data;
    QVector<QPieSeries* > vecSer;
    for(QPair<QString, double> p : chartData){
        QString name = p.first;
        double per = p.second;
        QPieSeries *curSer = new QPieSeries();
        curSer->setName(name);
        if(per == 100){
            curSer->append("Yes", per);
            goto pb;
        }
        if(per == 0){
            curSer->append("No", 100);
            goto pb;
        }
        curSer->append("Yes", per);
        curSer->append("No", 100 - per);

        pb:
        vecSer.push_back(curSer);
    }

    QDate cd = QDate::currentDate();

    QString dt = cd.toString("yyyy-MM-dd");

    DonutBreakdownChart *donutBreakdown = new DonutBreakdownChart();
    donutBreakdown->setAnimationOptions(QChart::AllAnimations);
    donutBreakdown->setTitle("Статистика выполнения на: " + dt);
    donutBreakdown->setTitleFont(QFont("Arial", 15));
    donutBreakdown->legend()->setAlignment(Qt::AlignRight);
    for(QPieSeries *cur : vecSer){
        donutBreakdown->addBreakdownSeries(cur, QColor(rand()%130+30, rand()%130+1, rand()%130+30, 120));
    }


    QChartView *chartView = new QChartView(donutBreakdown);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout->addWidget(chartView);
}

chart::~chart()
{
    delete ui;
}
