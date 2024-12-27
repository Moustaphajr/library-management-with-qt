#ifndef ADDEMPRUNT_H
#define ADDEMPRUNT_H

#include <QDialog>

namespace Ui {
class AddEmprunt;
}

class AddEmprunt : public QDialog
{
    Q_OBJECT

public:
    explicit AddEmprunt(QWidget *parent = nullptr);
    ~AddEmprunt();

private slots:
    void on_pushButton_clicked();

signals:
    void emitEmprunt();

private:
    Ui::AddEmprunt *ui;
};

#endif // ADDEMPRUNT_H
