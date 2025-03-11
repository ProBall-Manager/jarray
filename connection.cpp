#include <QDebug>
#include "connection.h"

Connection::Connection()
{
}

bool Connection::createconnect()
{
    bool test = false;

    // Only add the database if it doesn't already exist
    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

        db.setDatabaseName("Source_PRojet2A"); // Insert the name of the data source
        db.setUserName("proballManager"); // Insert the username
        db.setPassword("foot123456"); // Insert the password for this user

        qDebug() << "Attempting to connect to the database...";

        if (db.open()) {
            test = true;
            qDebug() << "Connection successful!";
        } else {
            qDebug() << "Connection failed: " << db.lastError().text();
            qDebug() << "Connection error details: " << db.lastError().driverText();
        }
    } else {
        // Connection already exists, just check if it's open
        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        if (db.isOpen() || db.open()) {
            test = true;
            qDebug() << "Using existing open connection";
        }
    }

    return test;
}

// Add a new function to get the database connection
QSqlDatabase Connection::getConnection()
{
    return QSqlDatabase::database("qt_sql_default_connection");
}




