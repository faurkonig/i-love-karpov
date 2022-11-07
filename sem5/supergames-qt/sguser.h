#ifndef SGUSER_H
#define SGUSER_H

#include <QString>
#include <QDateTime>
#include <QtSql/QSqlQuery>

class SgUser
{
public:
    SgUser(int id = 0,
           QString login = "", QString password = "", QString name = "",
           QDateTime date = QDateTime::fromSecsSinceEpoch(0));

    static SgUser fromSql(QSqlQuery &query);

    int id;
    QString login;
    QString password;
    QString name;
    QDateTime date;

    bool isGuest();
};

#endif // SGUSER_H
