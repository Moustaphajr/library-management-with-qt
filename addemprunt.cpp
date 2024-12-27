#include "addemprunt.h"
#include "ui_addemprunt.h"
#include "dbaccess.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <empruntmanager.h>

/**
 * @brief Constructeur de la classe AddEmprunt.
 *
 * Initialise l'interface utilisateur et configure les éléments associés à la fenêtre d'ajout d'emprunt.
 *
 * @param parent Widget parent de la fenêtre.
 */
AddEmprunt::AddEmprunt(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddEmprunt)
{
    ui->setupUi(this);
}

/**
 * @brief Destructeur de la classe AddEmprunt.
 *
 * Supprime l'interface utilisateur associée à la fenêtre d'ajout d'emprunt.
 */
AddEmprunt::~AddEmprunt()
{
    delete ui;
}

/**
 * @brief Slot appelé lorsque le bouton "Ajouter Emprunt" est cliqué.
 *
 * Cette fonction permet d'ajouter un nouvel emprunt dans la base de données si le livre est disponible.
 * Elle vérifie d'abord si le livre est disponible (`isAvailable` à "true") puis ajoute l'emprunt à la table `Emprunt`.
 * Si l'opération réussit, le statut du livre est mis à jour pour indiquer qu'il est emprunté (`isAvailable` à "false").
 * La fonction affiche également des messages de succès ou d'erreur en fonction du résultat des opérations.
 */
void AddEmprunt::on_pushButton_clicked()
{
    // Connexion à la base de données
    DBAccess dbaccess;
    dbaccess.open();

    QString id_membre, id_livre, date_emprunt, date_retour_prevue, date_retour_reelle;

    // Récupération des données saisies par l'utilisateur dans les champs de texte
    id_membre = ui->identifiantDuMembreLineEdit->text();
    id_livre = ui->identifantDuLivreLineEdit->text();
    date_emprunt = ui->dateDEmpruntLineEdit->text();
    date_retour_prevue = ui->dateDeRetourPrVueLineEdit->text();
    date_retour_reelle = ui->dateDeRetourRElleLineEdit->text();

    // Vérification de la disponibilité du livre dans la base de données
    QSqlQuery query(dbaccess.db);
    query.prepare("SELECT * FROM LIVRES WHERE id = :id_livre AND isAvailable = :status");
    query.bindValue(":id_livre", id_livre);
    query.bindValue(":status", "true");

    // Si le livre est disponible, on peut créer un nouvel emprunt
    if (query.exec() && query.next()) {

        QSqlQuery qry(dbaccess.db);

        // Préparation de la requête d'insertion d'un nouvel emprunt
        qry.prepare("INSERT INTO Emprunt (id_Membre, id_Livre, date_emprunt, date_retour_prevue, date_retour_reelle) "
                    "VALUES (:id_membre, :id_livre, :date_emprunt, :date_retour_prevue, :date_retour_reelle)");
        qry.bindValue(":id_membre", id_membre);
        qry.bindValue(":id_livre", id_livre);
        qry.bindValue(":date_emprunt", date_emprunt);
        qry.bindValue(":date_retour_prevue", date_retour_prevue);
        qry.bindValue(":date_retour_reelle", date_retour_reelle);

        // Exécution de la requête
        if (qry.exec()) {
            QMessageBox::information(this, "Succès", "Emprunt ajouté avec succès");
            qDebug() << "Emprunt fait avec succès";

            // Mise à jour du statut du livre pour indiquer qu'il n'est plus disponible
            QSqlQuery updateQuery(dbaccess.db);
            updateQuery.prepare("UPDATE LIVRES SET isAvailable = :isAvailable WHERE id = :id");
            updateQuery.bindValue(":isAvailable", "false");  // Le livre est maintenant emprunté
            updateQuery.bindValue(":id", id_livre);

            if (updateQuery.exec()) {
                qDebug() << "Statut du livre mis à jour avec succès à 'Non disponible'";
            } else {
                qDebug() << "Erreur lors de la mise à jour du statut du livre:" << updateQuery.lastError().text() << id_livre;
            }

            // Réinitialisation des champs du formulaire après l'ajout de l'emprunt
            ui->identifiantDuMembreLineEdit->setText("");
            ui->identifantDuLivreLineEdit->setText("");
            ui->dateDEmpruntLineEdit->setText("");
            ui->dateDeRetourPrVueLineEdit->setText("");
            ui->dateDeRetourRElleLineEdit->setText("");

            // Émettre un signal pour indiquer que l'emprunt a été ajouté (utile pour mettre à jour d'autres composants)
            emit emitEmprunt();

            // Ouvrir la fenêtre de gestion des emprunts
            EmpruntManager *empruntManager = new EmpruntManager(this);
            empruntManager->show();

            // Fermer la fenêtre actuelle après l'ajout de l'emprunt
            this->close();

        } else {
            // Affichage d'une erreur en cas d'échec de l'insertion
            qDebug() << "Erreur sur la requête: " << qry.lastError().text();
        }

    } else {
        // Si le livre est déjà emprunté, afficher un message d'erreur
        qDebug() << "Erreur sur la requête: " << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Le livre est déjà emprunté");
    }

    // Fermeture de la connexion à la base de données
    dbaccess.close();
}
