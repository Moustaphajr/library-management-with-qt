#include "reportgenerator.h"
#include "dbaccess.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ReportGenerator::ReportGenerator() {

        bookSatistics();
}




//retourne le nombre de livre

/**
 * @brief Retourne le nombre total de livres dans la base de données.
 *
 * Cette méthode se connecte à la base de données, exécute une requête SQL pour compter
 * le nombre total de livres dans la table LIVRES, puis ferme la connexion.
 *
 * @return Le nombre total de livres.
 */
int ReportGenerator::bookSatistics() {
    DBAccess dbaccess;
    dbaccess.open();

    QSqlQuery query(dbaccess.db);
    query.prepare("SELECT count(*) FROM LIVRES");

    int numbersOfBook=0 ;

    if(query.exec()) {
        if(query.next()) {
            numbersOfBook = query.value(0).toInt();
            qDebug() << "Requête exécutée avec succès. Nombre de livres :" << numbersOfBook;
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();

    }


    dbaccess.close();

      return numbersOfBook;
}
}




//retourne le nombre de membre

/**
 * @brief Retourne le nombre total de membres dans la base de données.
 *
 * Cette méthode se connecte à la base de données, exécute une requête SQL pour compter
 * le nombre total de membres dans la table users, puis ferme la connexion.
 *
 * @return Le nombre total de membres.
 */
int ReportGenerator::memberStatistics() {
    DBAccess dbaccess;
    dbaccess.open();

    QSqlQuery query(dbaccess.db);
    query.prepare("SELECT count(*) FROM users");

    int numberOfMembers = 0;

    if (query.exec() && query.next()) {
        numberOfMembers = query.value(0).toInt();
        qDebug() << "Requête exécutée avec succès. Nombre de membres :" << numberOfMembers;
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
    }

    dbaccess.close();
    return numberOfMembers;
}






//retourne les livres les plus empruntés

/**
 * @brief Retourne une liste des livres les plus empruntés.
 *
 * Cette méthode se connecte à la base de données, exécute une requête SQL pour récupérer
 * les livres les plus empruntés, et retourne une liste des livres avec les informations
 * suivantes : Titre, Date d'ajout, Auteur, Genre, et Nombre d'emprunts. Les résultats sont
 * triés par nombre d'emprunts en ordre décroissant.
 *
 * @return Une liste de livres, où chaque livre est représenté par un QVector de QStrings
 *         contenant les informations : Titre, Date d'ajout, Auteur, Genre, et Nombre d'emprunts.
 */
QVector<QVector<QString>> ReportGenerator::loadTopEmpruntedBook() {
    DBAccess dbaccess;
    dbaccess.open();

    QVector<QVector<QString>> bookList;

    QSqlQuery query(dbaccess.db);
    query.prepare(R"(
        SELECT L.Titre, L.Date_ajout, L.Auteur, L.Genre, COUNT(Em.id_livre) AS nombres_emprunts
        FROM Emprunt Em
        JOIN LIVRES L ON Em.id_livre = L.id
        GROUP BY L.Titre, L.Date_ajout, L.Auteur, L.Genre
        HAVING COUNT(Em.id_livre) > 1
        ORDER BY nombres_emprunts DESC
        LIMIT 3
    )");

    if (query.exec()) {
        while (query.next()) {
            QVector<QString> row;
            row.append(query.value(0).toString()); // Titre
            row.append(query.value(1).toString()); // Date ajout
            row.append(query.value(2).toString()); // Auteur
            row.append(query.value(3).toString()); // Genre
            row.append(query.value(4).toString()); // Nombre emprunts

            bookList.append(row);
        }
    } else {
        qDebug() << "Erreur lors de la récupération des livres:" << query.lastError().text();
    }

    dbaccess.close();
    return bookList;
}

