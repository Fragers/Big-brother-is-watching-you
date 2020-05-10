#ifndef SAVEDIA_H
#define SAVEDIA_H

#include <QDialog>

namespace Ui {
class saveDia;
}

class saveDia : public QDialog
{
    Q_OBJECT

public:
    explicit saveDia(QWidget *parent = nullptr);
    ~saveDia();
    bool acc = false;
    bool redj = true;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::saveDia *ui;
};

#endif // SAVEDIA_H
