#ifndef SGDEVELOPER_H
#define SGDEVELOPER_H

#include <QString>
#include <QDateTime>
#include <QtSql/QSqlQuery>

class SgDeveloper
{
public:
    SgDeveloper(int id = 0,
                QString email = "", QString password = "", QString name = "", QString description = "",
                QDateTime date = QDateTime::fromSecsSinceEpoch(0));

    static SgDeveloper fromSql(QSqlQuery &query);

    int id;
    QString email;
    QString password;
    QString name;
    QString description;
    QDateTime date;
};

#endif // SGDEVELOPER_H
