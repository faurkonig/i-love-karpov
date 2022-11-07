#include "databasecontainer.h"
#include "dialoghelper.h"

DatabaseContainer::DatabaseContainer(QWidget *parent, QSqlDatabase *newDb)
    : parent(parent), mainDatabase(newDb) {}

bool DatabaseContainer::checkDatabase()
{
    if (mainDatabase == nullptr || !mainDatabase->isOpen()) {
        DialogHelper::showDatabaseError(parent, "Нет подключения к базе данных");
        return false;
    }
    return true;
}
