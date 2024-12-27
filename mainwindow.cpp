#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include "dbaccess.h"
#include <QSqlQueryModel>
#include "reportgenerator.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statisticsTopBooks();
    BookNumber();
    MemberNumber();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::BookNumber(){

    ReportGenerator reportGenerator;

    int bookCount=reportGenerator.bookSatistics();

    QString stringBookCount = QString::number(bookCount);

    ui->label_3->setText(stringBookCount);

}

void MainWindow::MemberNumber() {


    ReportGenerator reportGenerator;

    int memberCount = reportGenerator.memberStatistics();

    QString stringMemberCount = QString::number(memberCount);

    ui->label_5->setText(stringMemberCount);
}






void MainWindow::on_pushButton_clicked()
{
    user->show();


}




void MainWindow::statisticsTopBooks(){


    ReportGenerator reportGenerator;
    QVector<QVector<QString>> topBooks = reportGenerator.loadTopEmpruntedBook();

    ui->tableWidget->setColumnCount(5);
    QStringList headers = { "Titre", "Date d'ajout", "Auteur", "Genre", "Nbrs d'emprunts" };
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    for (const QVector<QString>& book : topBooks) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < book.size(); ++col) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(book[col]));
        }
        row++;
    }
}








void MainWindow::on_pushButton_2_clicked()
{
    bookmanager->show();


}


void MainWindow::on_pushButton_3_clicked()
{
    empruntmanager->show();

}

