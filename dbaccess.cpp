#include "dbaccess.h"

/**
 * @brief Constructeur de la classe DBAccess.
 *
 * Initialise la connexion à la base de données SQLite en utilisant QSqlDatabase.
 * Le fichier de la base de données est spécifié dans le chemin d'accès.
 */
DBAccess::DBAccess() {
    // Création d'une instance de base de données SQLite
    db = QSqlDatabase::addDatabase("QSQLITE");
    // Spécification du chemin vers le fichier de la base de données
    db.setDatabaseName("C:/Users/DB_Browser/users_sqlite.db");
    // Message de confirmation dans la console
    qDebug("Accès à la base de données effectif !");
}

/**
 * @brief Ouvre la connexion à la base de données.
 *
 * Vérifie d'abord si la connexion est déjà ouverte.
 * Si elle ne l'est pas, elle tente de l'ouvrir.
 *
 * @return true si la connexion est ouverte avec succès, false sinon.
 */
bool DBAccess::open() {
    // Si la base de données est déjà ouverte, renvoie true
    if (db.isOpen() == true) return true;

    // Tentative d'ouverture de la connexion
    if (db.open() == true) {
        qDebug("Connexion ouverte avec la base de données");
        return true;
    } else {
        qDebug("Échec d'ouverture de la connexion avec la base de données");
        return false;
    }
}

/**
 * @brief Ferme la connexion à la base de données.
 *
 * Vérifie d'abord si la connexion est déjà fermée.
 * Si elle est ouverte, la connexion est fermée.
 */
void DBAccess::close() {
    // Si la base de données est déjà fermée, ne fait rien
    if (db.isOpen() == false) return;

    // Fermeture de la connexion
    db.close();
    // Message de confirmation dans la console
    qDebug("Fermeture de la connexion effectuée avec succès");
}

/**
 * @brief Destructeur de la classe DBAccess.
 *
 * Le destructeur est laissé vide car la gestion des ressources (fermeture de la connexion)
 * est effectuée manuellement par la méthode close(). Il n'est pas nécessaire de faire autre chose ici.
 */
DBAccess::~DBAccess() {}
