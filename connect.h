#ifndef CONNECT_H
#define CONNECT_H

#include <QSqlDatabase>
#include <QSqlQuery>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("highscore.sqlite");

    if (!db.open())
    {
        qDebug("Unable to establish a database connection");
        return false;
    }

    QSqlQuery query;

    query.exec("CREATE TABLE IF NOT EXISTS scores ("
               "start DATETIME, end DATETIME,"
               "playerWon TEXT, startingPlayer TEXT,"
               "score INTEGER"
               ")");

    return true;
}

#endif // CONNECT_H
