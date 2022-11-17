#ifndef DATABASECONTAINER_H
#define DATABASECONTAINER_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class DatabaseContainer
{
public:
    /// Конструктор
    DatabaseContainer(QWidget *parent, QSqlDatabase *newDb = nullptr);

protected:
    /// Виджет, от имени которого надо создавать диалоги об ошибках
    QWidget *parent;

    /// База данных, в которой будут исполнятся SQL запросы
    QSqlDatabase *mainDatabase;

    /// Проверка подключения к базе данных
    bool checkDatabase();

    /// Выполнение SQL запроса с дополнительными проверками и выводом ошибок при возникновении
    QSqlQuery execQuery(QString query, bool &ok);
};

#endif // DATABASECONTAINER_H
