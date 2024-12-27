#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H
#include <formcreatebook.h>
#include "editbookform.h"
#include "empruntmanager.h"

#include <QWidget>

namespace Ui {
class BookManager;
}

class BookManager : public QWidget
{
    Q_OBJECT

public:
    explicit BookManager(QWidget *parent = nullptr);
    ~BookManager();
     void displayBooks();

private slots:
    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void onEditButtonClicked(int row);
    void onDeleteButtonClicked(int row);



    void on_pushButton_3_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::BookManager *ui;
    Formcreatebook *formcreatebook=new Formcreatebook();
    EditBookForm *editForm=new EditBookForm();

    void setupTableView();
    EmpruntManager *empruntManager=new EmpruntManager();


};

#endif // BOOKMANAGER_H
