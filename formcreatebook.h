#ifndef FORMCREATEBOOK_H
#define FORMCREATEBOOK_H

#include <QDialog>

namespace Ui {
class Formcreatebook;
}

class Formcreatebook : public QDialog
{
    Q_OBJECT

public:
    explicit Formcreatebook(QWidget *parent = nullptr);
    ~Formcreatebook();

private slots:
    void on_pushButton_clicked();

signals:
    void bookAdded();

private:
    Ui::Formcreatebook *ui;

};

#endif // FORMCREATEBOOK_H
