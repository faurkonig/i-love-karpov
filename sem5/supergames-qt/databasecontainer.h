#ifndef DATABASECONTAINER_H
#define DATABASECONTAINER_H

#include <QWidget>
#include <QtSql/QSqlDatabase>

class DatabaseContainer
{
public:
    DatabaseContainer(QWidget *parent, QSqlDatabase *newDb = nullptr);

protected:
    QWidget *parent;

    QSqlDatabase *mainDatabase;

    bool checkDatabase();
};

#endif // DATABASECONTAINER_H
