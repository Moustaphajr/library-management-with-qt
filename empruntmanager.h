#ifndef EMPRUNTMANAGER_H
#define EMPRUNTMANAGER_H
#include "addemprunt.h"
#include "dbaccess.h"

#include <QWidget>

namespace Ui {
class EmpruntManager;
}

class EmpruntManager : public QWidget
{
    Q_OBJECT

public:
    explicit EmpruntManager(QWidget *parent = nullptr);
    ~EmpruntManager();

private slots:
    void on_pushButton_9_clicked();

private:
    Ui::EmpruntManager *ui;
    AddEmprunt *addemprunt =new  AddEmprunt();
    void loadEmprunts();
    void returnLivre(int row);


};

#endif // EMPRUNTMANAGER_H
