#ifndef FOMCREATEUSERDIALOG_H
#define FOMCREATEUSERDIALOG_H

#include <QDialog>

namespace Ui {
class FomCreateUserDialog;
}

class FomCreateUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FomCreateUserDialog(QWidget *parent = nullptr);
    ~FomCreateUserDialog();

private slots:
    void on_pushButton_clicked();


signals
:
void emitUser();

private:
    Ui::FomCreateUserDialog *ui;
};

#endif // FOMCREATEUSERDIALOG_H
