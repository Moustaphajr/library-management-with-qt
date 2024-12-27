#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "userwindow.h"
#include "bookmanager.h"
#include "empruntmanager.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();




private slots:
void on_pushButton_clicked();




void on_pushButton_2_clicked();

void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    Userwindow *user=new Userwindow();
    BookManager *bookmanager=new BookManager();
    EmpruntManager *empruntmanager=new EmpruntManager();


    void statisticsTopBooks();
    void BookNumber();
    void MemberNumber();
   /* void displayUsers();*/
};
#endif // MAINWINDOW_H
