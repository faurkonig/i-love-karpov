#include "utils/databasecontainer.h"
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

QSqlQuery DatabaseContainer::execQuery(QString query, bool &ok)
{
    ok = false;

    if (!checkDatabase()) return QSqlQuery();

    auto q = mainDatabase->exec(query);

    if (DialogHelper::isSqlError(q.lastError())) {
        DialogHelper::showSqlError(parent, q.lastError(), query);
    } else {
        ok = true;
    }

    return q;
}
