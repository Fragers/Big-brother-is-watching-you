#ifndef INITSTART_H
#define INITSTART_H

#include <QDialog>

namespace Ui {
class initStart;
}

class initStart : public QDialog
{
    Q_OBJECT

public:
    explicit initStart(QWidget *parent = nullptr);
    ~initStart();
    QString state = "close";

private slots:
    void on_OpenF_clicked();

    void on_NewF_clicked();

    void on_CloseF_clicked();

private:
    Ui::initStart *ui;
};

#endif // INITSTART_H
