#include "empruntmanager.h"
#include "ui_empruntmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>



EmpruntManager::EmpruntManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EmpruntManager)
{
    ui->setupUi(this);
    loadEmprunts();
    connect(addemprunt, &AddEmprunt::emitEmprunt, this, &EmpruntManager::loadEmprunts);
}

EmpruntManager::~EmpruntManager()
{
    delete ui;
}



/**
 * @brief Affiche la fenêtre d'ajout d'emprunt lorsque le bouton est cliqué.
 *
 * Cette fonction est déclenchée lorsqu'on clique sur le bouton identifié par `pushButton_9`.
 * Elle ouvre ou affiche la fenêtre ou le formulaire associé à l'objet `addemprunt`.
 */

void EmpruntManager::on_pushButton_9_clicked()
{
    addemprunt->show();

}


/**
 * @brief Charge et affiche la liste des emprunts dans le tableau de l'interface utilisateur.
 *
 * Cette fonction récupère les emprunts de la base de données où le livre n'est pas encore
 * retourné (statut `isAvailable` à "false") et les affiche dans un `QTableWidget`.
 * Chaque emprunt affiché inclut un bouton pour retourner le livre.
 *
 * La fonction se connecte à la base de données via `DBAccess`, récupère les informations
 * nécessaires, et les insère dans le tableau de l'interface utilisateur.
 *
 * La colonne "Action" du tableau contient un bouton "Retourner Livre" qui, une fois cliqué,
 * appelle la fonction `returnLivre` pour permettre le retour du livre.
 */

void EmpruntManager::loadEmprunts()
{
    DBAccess dbaccess;
    dbaccess.open();

    ui->tableWidget->clearContents(); // Efface le contenu du tableau mais conserve la structure
    ui->tableWidget->setRowCount(0);  // Réinitialise le nombre de lignes

    ui->tableWidget->setColumnCount(8);
    QStringList headers = { "ID", "Nom", "Email", "ID Livre", "Titre Livre", "Date Emprunt", "Date Retour", "Action" };
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    QSqlQuery query(dbaccess.db);
    query.prepare("SELECT Em.id, e.Nom, e.Email, Em.id_Livre, L.Titre, Em.date_emprunt,  Em.date_retour_reelle "
                  "FROM users e "
                  "INNER JOIN Emprunt Em ON e.id = Em.id_membre "
                  "JOIN Livres L ON Em.id_livre = L.id "
                  "WHERE L.isAvailable = :status  and Em.isReturn=:isReturn");
    query.bindValue(":status", "false");
    query.bindValue(":isReturn","false");

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            ui->tableWidget->insertRow(row);

            QString id = query.value(0).toString();
            QString nom = query.value(1).toString();
            QString email = query.value(2).toString();
            QString idLivre = query.value(3).toString();
            QString titreLivre = query.value(4).toString();
            QString dateEmprunt = query.value(5).toString();
            QString dateRetourReelle = query.value(6).toString();

            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(nom));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(email));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(idLivre));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(titreLivre));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(dateEmprunt));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(dateRetourReelle));

            QPushButton *returnButton = new QPushButton("Retourner Livre");
            returnButton->setStyleSheet("background-color: blue; color: white; margin:3px;");

            connect(returnButton, &QPushButton::clicked, this, [this, row]() {
                this->returnLivre(row);
            });

            ui->tableWidget->setCellWidget(row, 7, returnButton);
            row++;
        }
    } else {
        qDebug() << "Erreur lors de la récupération des emprunts: " << query.lastError().text();
    }

    dbaccess.close();
}




/**
 * @brief Cette fonction permet de retourner un livre emprunté.
 *
 * Cette fonction met à jour la base de données pour indiquer que le livre
 * est désormais disponible, et elle supprime l'entrée correspondante de
 * l'emprunt dans la table Emprunt.
 *
 * @param row La ligne du tableau contenant l'emprunt à retourner. La ligne
 * spécifie l'entrée dans le `tableWidget` qui contient les informations
 * sur l'emprunt, notamment l'ID du livre et l'ID de l'emprunt.
 *
 * - Met à jour la disponibilité du livre dans la table LIVRES.
 * - Supprime l'entrée correspondante dans la table Emprunt après un retour réussi.
 * - Affiche des messages d'information ou d'erreur selon le résultat de l'opération.
 */


void EmpruntManager::returnLivre(int row)
{
    DBAccess dbaccess;
    dbaccess.open();

    QString item = ui->tableWidget->item(row, 3)->text();
    QString idEmprunt = ui->tableWidget->item(row, 0)->text();

    int idLivre = item.toInt();
    QString isAvailable = "true";

    // Débogage pour vérifier l'ID du livre et de l'emprunt
    qDebug() << "ID Livre: " << idLivre << ", ID Emprunt: " << idEmprunt;

    // Requête de mise à jour de la disponibilité du livre
    QSqlQuery query(dbaccess.db);
    query.prepare("UPDATE LIVRES SET isAvailable = :isAvailable WHERE id = :id");
    query.bindValue(":isAvailable", isAvailable);
    query.bindValue(":id", idLivre);

    if (query.exec()) {
        qDebug() << "Livre mis à jour avec succès dans la table LIVRES";

        // Suppression de l'emprunt
        QSqlQuery update(dbaccess.db);
        update.prepare("UPDATE Emprunt set isReturn=:isReturn where id=:id");
        update.bindValue(":isReturn","true");
        update.bindValue(":id", idEmprunt);

        if (update.exec()) {
            qDebug() << "Emprunt supprimé avec succès";
            QMessageBox::information(this, "Retour du livre", "Le livre a été retourné avec succès !");

            // Rechargez la liste des emprunts
            loadEmprunts();
        } else {
            qDebug() << "Erreur lors de la suppression de l'emprunt: " << update.lastError().text();
            QMessageBox::critical(this, "Erreur", "Une erreur s'est produite lors du retour du livre.");
        }
    } else {
        qDebug() << "Erreur lors de la mise à jour de la disponibilité du livre: " << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de mettre à jour la disponibilité du livre.");
    }

    dbaccess.close();
}











