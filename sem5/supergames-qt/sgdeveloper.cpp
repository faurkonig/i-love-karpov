#include "sgdeveloper.h"

#include <QVariant>

SgDeveloper::SgDeveloper(int id, QString email, QString password, QString name, QString description, QDateTime date)
    : id(id), email(email), password(password), name(name), description(description), date(date)
{

}
