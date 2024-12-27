#ifndef DBACCESS_H
#define DBACCESS_H
#include <QtSql/QSqlDatabase>

class DBAccess
{

public :
    QSqlDatabase db;


public:

    QSqlDatabase database(){return db;}
     DBAccess();
    ~DBAccess();


    bool open();
    void close();

};

#endif // DBACCESS_H
