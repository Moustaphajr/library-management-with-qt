#include "formcreatebook.h"
#include "ui_formcreatebook.h"
#include <dbaccess.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include "dbaccess.h"
#include <QSqlQueryModel>
#include <QMessageBox>
#include <bookmanager.h>


Formcreatebook::Formcreatebook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Formcreatebook)
{
    ui->setupUi(this);

}

Formcreatebook::~Formcreatebook()
{
    delete ui;
}


/**
 * @brief Slot appelé lorsque le bouton "Ajouter" est cliqué.
 *
 * Ce slot récupère les données saisies par l'utilisateur dans les champs de texte,
 * vérifie que tous les champs sont remplis, et tente d'ajouter un nouveau livre
 * à la base de données. Si l'ajout est réussi, les champs sont vidés, un message de succès
 * est affiché, et une nouvelle instance de `BookManager` est ouverte. En cas d'erreur, un message
 * d'erreur est affiché.
 */

void Formcreatebook::on_pushButton_clicked()
{

    DBAccess dbaccess;

    dbaccess.open();

        QString Auteur, Genre, Date_ajout, Titre;

        // Récupération des valeurs à partir des champs d'entrée
        Auteur = ui->auteurLineEdit->text();
        Genre = ui->genreLineEdit->text();
        Date_ajout = ui->dateDAjoutLineEdit->text();
        Titre = ui->titreLineEdit->text();

        // Vérification de la validité des champs
        if (Auteur.isEmpty() || Genre.isEmpty() || Date_ajout.isEmpty() || Titre.isEmpty()) {
            QMessageBox::warning(this, "Invalid", "Remplissez tous les champs.");
            return;
        }

        // Préparation de l'insertion des données dans la base
        QSqlQuery query;
        query.prepare("INSERT INTO LIVRES (Auteur, Genre, Date_ajout, Titre) "
                      "VALUES (:Auteur, :Genre, :Date_ajout, :Titre)");

        query.bindValue(":Auteur", Auteur);
        query.bindValue(":Genre", Genre);
        query.bindValue(":Date_ajout", Date_ajout);
        query.bindValue(":Titre", Titre);

        // Exécution de la requête et gestion des erreurs
        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur de la base de donnée", "échoue de l'insertion: " + query.lastError().text());
        } else {
            QMessageBox::information(this, "Succéss", "Livre ajouté avec succés !");

            // Effacer les champs après l'ajout
            ui->auteurLineEdit->clear();
            ui->genreLineEdit->clear();
            ui->dateDAjoutLineEdit->clear();
            ui->titreLineEdit->clear();


             emit bookAdded();

            BookManager *bookManager = new BookManager(this);
            bookManager->show();



            this->close();



        }

        dbaccess.close();

}

