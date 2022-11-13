#include "sguser.h"

#include <QVariant>

SgUser::SgUser(int id, QString login, QString password, QString name, QDateTime date)
    : id(id), login(login), password(password), name(name), date(date)
{
}

bool SgUser::isGuest()
{
    return id == 0;
}
