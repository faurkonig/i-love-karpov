#include "sgdeveloper.h"

#include <QVariant>

SgDeveloper::SgDeveloper(int id, QString email, QString password, QString name, QString description, QDateTime date)
    : id(id), email(email), password(password), name(name), description(description), date(date)
{

}

SgDeveloper SgDeveloper::fromSql(QSqlQuery &query)
{
    return SgDeveloper(
                query.value(0).toInt(),
                query.value(1).toString(),
                query.value(2).toString(),
                query.value(3).toString(),
                query.value(4).toString(),
                query.value(5).toDateTime()
                );
}
