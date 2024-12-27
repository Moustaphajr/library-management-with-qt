#include "userwindow.h"
#include "ui_userwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include "dbaccess.h"
#include <QHBoxLayout> // Include this for QHBoxLayout
#include <QPushButton>
#include <editformuser.h>
#include <QMessageBox>


Userwindow::Userwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Userwindow)
{
    ui->setupUi(this);
    loadUsers();
     connect(formDialog, &FomCreateUserDialog::emitUser, this, &Userwindow::loadUsers);
}

Userwindow::~Userwindow()
{
    delete ui;
}


/**
 * @brief Charge les données des utilisateurs et les affiche dans un QTableWidget.
 *
 * Cette méthode se connecte à la base de données, récupère les informations des utilisateurs
 * depuis la table `users`, puis les affiche dans un `QTableWidget`. Les données incluent les
 * colonnes ID, Nom, Date d'ajout, Email, Sexe, et des boutons pour les actions Modifier et Supprimer.
 *
 * La méthode effectue les opérations suivantes :
 * - Ouvre la connexion à la base de données.
 * - Exécute une requête SQL pour récupérer toutes les entrées de la table `users`.
 * - Configure le `QTableWidget` pour afficher les données des utilisateurs.
 * - Ajoute des boutons "Modifier" et "Supprimer" pour chaque utilisateur.
 * - Connecte les boutons aux slots appropriés pour gérer les actions.
 * - Ajuste la largeur des colonnes du `QTableWidget`.
 * - Ferme la connexion à la base de données.
 */

void Userwindow::loadUsers()
{
    // Ouvrir la connexion à la base de données
    DBAccess dbaccess;
    dbaccess.open();

    // Requête pour récupérer les données depuis la table users
    QSqlQuery qry(dbaccess.db);
    qry.prepare("SELECT * FROM users");

    if (qry.exec())
    {
        // Configurer le QTableWidget
        ui->tableWidget->clear(); // Effacer les données existantes
        ui->tableWidget->setRowCount(0); // Réinitialiser les lignes

        // Définir les en-têtes des colonnes
        ui->tableWidget->setColumnCount(6); // 5 colonnes pour les données + 1 pour les boutons
        ui->tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Date d'ajout", "Email", "Sexe", "Actions"});

        int row = 0;
        while (qry.next())
        {
            // Ajouter une nouvelle ligne au QTableWidget
            ui->tableWidget->insertRow(row);

            // Remplir les cellules avec les données de la base de données
            for (int col = 0; col < 5; ++col)
            {
                ui->tableWidget->setItem(row, col, new QTableWidgetItem(qry.value(col).toString()));
            }

            // Ajouter les boutons "Modifier" et "Supprimer"
            QPushButton *editButton = new QPushButton("Modifier");
            QPushButton *deleteButton = new QPushButton("Supprimer");

            // Style les boutons
            editButton->setStyleSheet("background-color: blue; color: white;");
            deleteButton->setStyleSheet("background-color: red; color: white;");

            // Créer un widget pour contenir les boutons
            QWidget *buttonWidget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(buttonWidget);
            layout->addWidget(editButton);
            layout->addWidget(deleteButton);
            layout->setContentsMargins(0, 0, 0, 0);
            buttonWidget->setLayout(layout);

            // Ajouter le widget contenant les boutons à la colonne des actions
            ui->tableWidget->setCellWidget(row, 5, buttonWidget);

            // Connecter les boutons aux slots
            connect(editButton, &QPushButton::clicked, this, [this, row]() {
                this->onEditButtonClicked(row);
            });
            connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
                this->onDeleteButtonClicked(row);
            });

            ++row;
        }

        // Ajuster la largeur des colonnes si nécessaire
        ui->tableWidget->resizeColumnsToContents();
    }
    else
    {
        qDebug() << "Échec de l'exécution de la requête.";
    }

    // Fermer la connexion à la base de données
    dbaccess.close();
}



/**
 * @brief Slot appelé lorsque le bouton "Modifier" est cliqué pour une ligne spécifique.
 *
 * Cette méthode est responsable de l'ouverture d'un formulaire d'édition pour l'utilisateur sélectionné.
 * Elle récupère les informations de l'utilisateur à partir de la ligne spécifiée dans le QTableWidget,
 * affiche le formulaire d'édition, et connecte le signal de mise à jour du formulaire à la mise à jour
 * des données dans la base de données.
 *
 * @param row Numéro de la ligne dans le QTableWidget où se trouve l'utilisateur à modifier.
 */


void Userwindow::onEditButtonClicked(int row) {

    QString  id=ui->tableWidget->item(row,0)->text();
    QString name=ui->tableWidget->item(row,1)->text();
    QString date_ajout=ui->tableWidget->item(row,2)->text();
    QString email=ui->tableWidget->item(row,3)->text();
    QString sexe=ui->tableWidget->item(row,4)->text();

    EditformUser *editFormuser =new EditformUser();
    editFormuser->show();
    editFormuser->setUserData(name,date_ajout,email,sexe);

    connect(editFormuser,&EditformUser::updateClick,this,[this,editFormuser,id](){


        DBAccess dbaccess;

        QString name=editFormuser->userName();
        QString date=editFormuser->userAddDate();
        QString email=editFormuser->userEmail();
        QString sexe=editFormuser->userSexe();


        dbaccess.open();

        QSqlQuery qry(dbaccess.db);

        qry.prepare("UPDATE users set Nom= :Nom , date_inscription= :Date, Email= :Email,Sexe= :Sexe where id= :id");
        qry.bindValue(":Nom",name);
        qry.bindValue(":Date",date);
        qry.bindValue(":Email",email);
        qry.bindValue(":Sexe",sexe);
        qry.bindValue(":id",id);

        if (qry.exec()) {
            qDebug() << "User updated sucessfully";
             QMessageBox::information(this, "Succés", "utilisateur modifié avec succés");
            loadUsers();
        } else {
            qDebug() << "failed to update user: " << qry.lastError().text();
        }


        dbaccess.close();
        editFormuser->deleteLater();

    });


}


/**
 * @brief Slot appelé lorsque le bouton "Supprimer" est cliqué pour une ligne spécifique.
 *
 * Cette méthode est responsable de la suppression de l'utilisateur sélectionné dans la base de données.
 * Elle affiche une boîte de dialogue de confirmation pour s'assurer que l'utilisateur souhaite réellement
 * supprimer l'enregistrement avant d'exécuter la requête de suppression.
 *
 * @param row Numéro de la ligne dans le QTableWidget où se trouve l'utilisateur à supprimer.
 */
void Userwindow::onDeleteButtonClicked(int row) {

    QString id=ui->tableWidget->item(row,0)->text();



    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Suprrimer un utilisateur", "es tu sur de vouloir supprimer?",
                                  QMessageBox::Yes | QMessageBox::No);


    if(reply == QMessageBox::Yes){

        DBAccess dbaccess;
        dbaccess.open();
        QSqlQuery qry(dbaccess.db);



        qry.prepare("DELETE FROM users WHERE id= :id");
        qry.bindValue(":id",id);

        if (qry.exec()) {
            qDebug() << "utilisateur supprimer avec succés";
            loadUsers();
        } else {
            qDebug() << "suprression échouée: " << qry.lastError().text();
        }

        dbaccess.close();

    }




}

void Userwindow::on_pushButton_9_clicked()
{
    formDialog->show();

}


    void Userwindow::on_pushButton_clicked()
    {
        DBAccess dbaccess;
        dbaccess.open();

        QString searchText = ui->lineEdit->text();

        QSqlQuery qry(dbaccess.db);
        qry.prepare("SELECT * FROM users WHERE id = :id OR nom LIKE :nom");
        qry.bindValue(":id", searchText);
        qry.bindValue(":nom", "%" + searchText + "%");

        if (qry.exec())
        {
            // Configure the QTableWidget
            ui->tableWidget->clear(); // Clear existing data
            ui->tableWidget->setRowCount(0); // Reset rows

            // Define column count (5 columns for data + 1 column for buttons)
            ui->tableWidget->setColumnCount(6);
            ui->tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Date d'ajout", "Email", "Sexe", "Actions"});

            int row = 0;
            while (qry.next())
            {
                // Add a new row to the QTableWidget
                ui->tableWidget->insertRow(row);

                // Populate cells with data from the database
                for (int col = 0; col < 5; ++col)
                {
                    ui->tableWidget->setItem(row, col, new QTableWidgetItem(qry.value(col).toString()));
                }

                // Create "Edit" and "Delete" buttons
                QPushButton *editButton = new QPushButton("Modifier");
                QPushButton *deleteButton = new QPushButton("Supprimer");

                // Style the buttons
                editButton->setStyleSheet("background-color: blue; color: white;");
                deleteButton->setStyleSheet("background-color: red; color: white;");

                // Create a widget to contain the buttons
                QWidget *buttonWidget = new QWidget();
                QHBoxLayout *layout = new QHBoxLayout(buttonWidget);
                layout->addWidget(editButton);
                layout->addWidget(deleteButton);
                layout->setContentsMargins(0, 0, 0, 0);
                buttonWidget->setLayout(layout);

                // Add the widget with buttons to the actions column
                ui->tableWidget->setCellWidget(row, 5, buttonWidget);

                // Connect the buttons to slots
                connect(editButton, &QPushButton::clicked, this, [this, row]() {
                    this->onEditButtonClicked(row);
                });
                connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
                    this->onDeleteButtonClicked(row);
                });

                ++row;
            }

            // Adjust column widths if needed
            ui->tableWidget->resizeColumnsToContents();
        }
        else
        {
            qDebug() << "Query failed to execute";
        }

        dbaccess.close();
    }




    void Userwindow::on_pushButton_3_clicked()
    {
        bookManager->show();

    }


    void Userwindow::on_pushButton_4_clicked()
    {
        empruntManager->show();

    }


    void Userwindow::on_pushButton_7_clicked()
    {
        loadUsers();
        ui->lineEdit->setText("");
    }

