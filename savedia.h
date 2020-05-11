#ifndef SAVEDIA_H
#define SAVEDIA_H

#include <QDialog>
#include <QString>
namespace Ui {
class saveDia;
}

class saveDia : public QDialog
{
    Q_OBJECT

public:
    explicit saveDia(QWidget *parent = nullptr);
    ~saveDia();
    QString state = "cancel";
private slots:

    void on_Cancel_clicked();

    void on_Close_clicked();

    void on_SaveAll_clicked();

private:
    Ui::saveDia *ui;
};

#endif // SAVEDIA_H
