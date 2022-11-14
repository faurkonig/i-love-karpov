#ifndef DATABASECONTAINER_H
#define DATABASECONTAINER_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class DatabaseContainer
{
public:
    DatabaseContainer(QWidget *parent, QSqlDatabase *newDb = nullptr);

protected:
    QWidget *parent;

    QSqlDatabase *mainDatabase;

    bool checkDatabase();

    QSqlQuery execQuery(QString query, bool &ok);
};

#endif // DATABASECONTAINER_H
