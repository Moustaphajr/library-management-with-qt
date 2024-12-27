#include "fomcreateuserdialog.h"
#include "ui_fomcreateuserdialog.h"
#include "dbaccess.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <userwindow.h>


FomCreateUserDialog::FomCreateUserDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FomCreateUserDialog)
{
    ui->setupUi(this);
}

FomCreateUserDialog::~FomCreateUserDialog()
{
    delete ui;
}


/**
 * @brief Slot appelé lorsque le bouton "Ajouter" est cliqué.
 *
 * Ce slot récupère les données saisies par l'utilisateur dans les champs de texte,
 * vérifie que tous les champs sont remplis, et tente d'ajouter un nouvel utilisateur
 * à la base de données. Si l'ajout est réussi, les champs sont vidés, un message de succès
 * est affiché, et une fenêtre utilisateur est ouverte. En cas d'erreur, un message d'erreur
 * est affiché.
 */
void FomCreateUserDialog::on_pushButton_clicked()
{
    //récupérer les entrés de l'utilisateur
    QString Nom = ui->nomLineEdit->text();
    QString date_inscription = ui->dateDInscriptionLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString sexe = ui->sexeLineEdit->text();

    // vérifier si tous  les champs sont remplis
    if (Nom.isEmpty() || date_inscription.isEmpty() || email.isEmpty() || sexe.isEmpty()) {
        QMessageBox::warning(this, "Erreur input", "remplissez tous les champs");
        return;
    }


    // Step 3: vérifier  l'ouverture de la connection
    DBAccess dbaccess;
    if (!dbaccess.open()) {
        QMessageBox::critical(this, "data", "Echec de l'ouverture de la base de donnée");
        return;
    }

    // Step 4: Preparer la requete
    QSqlQuery query;
    query.prepare("INSERT INTO users (Nom, date_inscription, email, sexe) "
                  "VALUES (:Nom, :date_inscription, :email, :sexe)");
    query.bindValue(":Nom", Nom);
    query.bindValue(":date_inscription", date_inscription);
    query.bindValue(":email", email);
    query.bindValue(":sexe", sexe);

    // Step 5: Executer la requete
    if (query.exec()) {
        ui->nomLineEdit->clear();
        ui->dateDInscriptionLineEdit->clear();
        ui->emailLineEdit->clear();
        ui->sexeLineEdit->clear();
        QMessageBox::information(this, "Succés", "Utilisateur ajouté avec succés");

        emit emitUser();

        Userwindow *userWindow = new Userwindow(this);
        userWindow->show();
        this->close();

    } else {
        QMessageBox::critical(this, "Erreur base donnée", "Echec de l'ajout: " + query.lastError().text());
    }

    // Step 6: fermeture de la connection
    dbaccess.close();
}

