#ifndef CHART_H
#define CHART_H

#include <QDialog>
#include <QChart>
namespace Ui {
class chart;
}

class chart : public QDialog
{
    Q_OBJECT

public:
    explicit chart(QWidget *parent = nullptr, QVector<QPair<QString, double>> chartData = QVector<QPair<QString, double>>());
    ~chart();
    QVector<QPair<QString, double>> data;
private:
    Ui::chart *ui;
};

#endif // CHART_H
