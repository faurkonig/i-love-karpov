#include "sguser.h"

#include <QVariant>

SgUser::SgUser(int id, QString login, QString password, QString name, QDateTime date)
    : id(id), login(login), password(password), name(name), date(date)
{
}

SgUser SgUser::fromSql(QSqlQuery &query)
{
    return SgUser(
                query.value(0).toInt(),
                query.value(1).toString(),
                query.value(2).toString(),
                query.value(3).toString(),
                query.value(4).toDateTime()
                );
}

bool SgUser::isGuest()
{
    return id == 0;
}
