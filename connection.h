#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Connection
{
public:
    Connection();
    bool createconnect();
    QSqlDatabase getConnection(); // Added new function to get the connection
};

#endif // CONNECTION_H
