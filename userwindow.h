#ifndef USERWINDOW_H
#define USERWINDOW_H
#include "fomcreateuserdialog.h"
#include "editformuser.h"
#include "bookmanager.h"
#include "empruntmanager.h"


#include <QWidget>

namespace Ui {
class Userwindow;
}

class Userwindow : public QWidget
{
    Q_OBJECT

public:
    explicit Userwindow(QWidget *parent = nullptr);
    ~Userwindow();
    void loadUsers();

private slots:
    void on_pushButton_9_clicked();

    void on_pushButton_clicked();
    void onEditButtonClicked(int row);
    void onDeleteButtonClicked(int row);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Userwindow *ui;
    FomCreateUserDialog *formDialog=new FomCreateUserDialog();
    EditformUser *Editform =new EditformUser();
    BookManager *bookManager=new BookManager();
    EmpruntManager *empruntManager=new EmpruntManager();



};

#endif // USERWINDOW_H
