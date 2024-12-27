#include "bookmanager.h"
#include "ui_bookmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include "dbaccess.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "userwindow.h"

BookManager::BookManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BookManager)
{
    ui->setupUi(this);
    displayBooks();
    connect(formcreatebook, &Formcreatebook::bookAdded, this, &BookManager::displayBooks);
}

BookManager::~BookManager()
{
    delete ui;
}





/**
 * @brief Affiche la liste des livres dans le tableau de l'interface.
 *
 * Cette fonction interroge la base de données pour récupérer tous les livres,
 * puis les affiche dans un tableau avec des boutons pour les modifier ou les supprimer.
 */

void BookManager::displayBooks()
{

    DBAccess dbaccess;
    dbaccess.open();

    // Query to get all books
    QSqlQuery qry(dbaccess.db);
    qry.prepare("SELECT  *FROM LIVRES");

    if (qry.exec())
    {

        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);


        ui->tableWidget->setColumnCount(6);
        ui->tableWidget->setHorizontalHeaderLabels({"ID",  "Auteur", "Genre", "Date d'ajout","Titre", "Actions"});

        int row = 0;
        while (qry.next())
        {

            ui->tableWidget->insertRow(row);


            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(qry.value(0).toString())); // ID
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(qry.value(1).toString())); // Titre
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(qry.value(2).toString())); // Auteur
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(qry.value(3).toString())); // Genre
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(qry.value(4).toString())); // Date


            QPushButton *editButton = new QPushButton("Modifier");
            QPushButton *deleteButton = new QPushButton("Supprimer");


            editButton->setStyleSheet("background-color: blue; color: white; margin:5px;border-radius:5px; ");
            deleteButton->setStyleSheet("background-color: red; color: white;margin:5px; border-radius:5px; ");




            QWidget *buttonWidget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(buttonWidget);
            layout->addWidget(editButton);
            layout->addWidget(deleteButton);
            layout->setContentsMargins(0, 0, 0, 0);
            buttonWidget->setLayout(layout);


            ui->tableWidget->setCellWidget(row, 5, buttonWidget);


            connect(editButton, &QPushButton::clicked, this, [this, row]() {
                this->onEditButtonClicked(row);
            });
            connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
                this->onDeleteButtonClicked(row);
            });

            ++row;
        }


        ui->tableWidget->resizeColumnsToContents();

        qDebug() << "Books successfully displayed!";
    }
    else
    {
        qDebug() << "Failed to execute query: " << qry.lastError().text();
    }


    dbaccess.close();
}


/**
 * @brief Recherche des livres selon le texte entré dans la barre de recherche.
 *
 * Cette fonction filtre les livres dans la base de données en fonction de l'auteur, du titre ou du genre,
 * puis met à jour le tableau avec les résultats correspondants.
 */


void BookManager::on_pushButton_8_clicked()
{
    // ouverture de la base de donnée
    DBAccess dbaccess;
    dbaccess.open();

    QString searchText = ui->lineEdit->text();

    // requete pour la recherche
    QSqlQuery qry(dbaccess.db);
    qry.prepare("SELECT * FROM LIVRES WHERE Auteur LIKE :Auteur OR Titre LIKE :Titre OR Genre LIKE :Genre");

    qry.bindValue(":Auteur", "%" + searchText + "%");
    qry.bindValue(":Titre", "%" + searchText + "%");
    qry.bindValue(":Genre", "%" + searchText + "%");

    if (qry.exec())
    {

        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);


        ui->tableWidget->setColumnCount(6);
        ui->tableWidget->setHorizontalHeaderLabels({"ID", "Titre", "Auteur", "Genre", "Date", "Actions"});

        int row = 0;
        while (qry.next())
        {

            ui->tableWidget->insertRow(row);


            for (int col = 0; col < 5; ++col)
            {
                ui->tableWidget->setItem(row, col, new QTableWidgetItem(qry.value(col).toString()));
            }


            QPushButton *editButton = new QPushButton("Modifier");
            QPushButton *deleteButton = new QPushButton("Suprrimer");


            editButton->setStyleSheet("background-color: blue; color: white; margin:5px;border-radius:5px;");
            deleteButton->setStyleSheet("background-color: red; color: white;margin:5px; border-radius:5px;");


            QWidget *buttonWidget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(buttonWidget);
            layout->addWidget(editButton);
            layout->addWidget(deleteButton);
            layout->setContentsMargins(0, 0, 0, 0);
            buttonWidget->setLayout(layout);


            ui->tableWidget->setCellWidget(row, 5, buttonWidget);

            // Connecter les slots
            connect(editButton, &QPushButton::clicked, this, [this, row]() {
                this->onEditButtonClicked(row);
            });
            connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
                this->onDeleteButtonClicked(row);
            });

            ++row;
        }


        ui->tableWidget->resizeColumnsToContents();

        qDebug() << "Nombre de ligne retrouvée:" << ui->tableWidget->rowCount();
    }
    else
    {
        qDebug() << "Erreur de la requete";
    }

    dbaccess.close();
}




/**
 * @brief Slot appelé lorsqu'un bouton "Modifier" est cliqué.
 *
 * Permet de modifier un livre en affichant un formulaire de modification avec les données actuelles.
 *
 * @param row Numéro de la ligne du livre à modifier.
 */


void BookManager::onEditButtonClicked(int row)
{

    QString id = ui->tableWidget->item(row, 0)->text();
    QString title = ui->tableWidget->item(row, 4)->text();
    QString author = ui->tableWidget->item(row, 1)->text();
    QString genre = ui->tableWidget->item(row, 2)->text();
    QString date = ui->tableWidget->item(row, 3)->text();


    EditBookForm *editForm = new EditBookForm(this);
    editForm->setBookData(title, author, genre, date);


    connect(editForm, &EditBookForm::saveClicked, this, [this, editForm, id]() {
        QString title = editForm->bookTitle();
        QString author = editForm->bookAuthor();
        QString genre = editForm->bookGenre();
        QString date = editForm->bookDate();


        DBAccess dbaccess;
        dbaccess.open();
        QSqlQuery qry(dbaccess.db);
        qry.prepare("UPDATE LIVRES SET  Auteur = :Auteur, Genre = :Genre, Date_ajout = :Date, Titre=:Titre WHERE ID = :ID");

        qry.bindValue(":Auteur", author);
        qry.bindValue(":Genre", genre);
        qry.bindValue(":Date", date);
        qry.bindValue(":Titre",title);
        qry.bindValue(":ID", id);

        if (qry.exec()) {
            qDebug() << "livre modifié avec succés";
              QMessageBox::information(this, "Succés", "Livre modifié avec succés");
            displayBooks();
        } else {
            qDebug() << "Erreur lors de la modification: " << qry.lastError().text();
        }

        dbaccess.close();
        editForm->deleteLater();
    });

    editForm->exec();
}





/**
 * @brief Slot appelé lorsqu'un bouton "Supprimer" est cliqué.
 *
 * Cette fonction supprime un livre de la base de données après confirmation de l'utilisateur.
 *
 * @param row Numéro de la ligne du livre à supprimer.
 */
void BookManager::onDeleteButtonClicked(int row)
{
    QString id = ui->tableWidget->item(row, 0)->text();

    // Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Supprimer un livre", "vous etes sur de supprimer?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {

        DBAccess dbaccess;
        dbaccess.open();
        QSqlQuery qry(dbaccess.db);
        qry.prepare("DELETE FROM LIVRES WHERE ID = :ID");
        qry.bindValue(":ID", id);

        if (qry.exec()) {
            qDebug() << "Livre supprimer avec succés";
            QMessageBox::information(this, "Succés", "Livre supprimer avec succés");
            displayBooks();
        } else {
            qDebug() << "Echec de la supression: " << qry.lastError().text();
        }

        dbaccess.close();
    }
}

void BookManager::on_pushButton_9_clicked()
{
    formcreatebook->show();

}




void BookManager::on_pushButton_3_clicked()
{
    empruntManager->show();
}


void BookManager::on_pushButton_10_clicked()
{
    displayBooks();
    ui->lineEdit->setText("");
}

