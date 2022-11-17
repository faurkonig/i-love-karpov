#ifndef SGUSER_H
#define SGUSER_H

#include <QString>
#include <QDateTime>
#include <QtSql/QSqlQuery>

/// Универсальный класс, содержащий всю информации о пользователе
class SgUser
{
public:
    /// Конструктор
    SgUser(int id = 0,
           QString login = "", QString password = "", QString name = "",
           QDateTime date = QDateTime::fromSecsSinceEpoch(0));

    int id;
    QString login;
    QString password;
    QString name;
    QDateTime date;

    bool isGuest();
};

#endif // SGUSER_H
