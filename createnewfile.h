#ifndef CREATENEWFILE_H
#define CREATENEWFILE_H

#include <QDialog>
#include <QString>
#include <QSet>
namespace Ui {
class CreateNewFile;
}

class CreateNewFile : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNewFile(QWidget *parent = nullptr);
    ~CreateNewFile();
    QString fileName = "";
private slots:
    void on_Cancel_clicked();

    void on_Create_clicked();


private:
    Ui::CreateNewFile *ui;
};

#endif // CREATENEWFILE_H
