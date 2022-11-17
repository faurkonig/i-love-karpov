#ifndef SGDEVELOPER_H
#define SGDEVELOPER_H

#include <QString>
#include <QDateTime>
#include <QtSql/QSqlQuery>

/// Универсальный класс, содержащий всю информации о разработчике
class SgDeveloper
{
public:
    /// Конструктор
    SgDeveloper(int id = 0,
                QString email = "", QString password = "", QString name = "", QString description = "",
                QDateTime date = QDateTime::fromSecsSinceEpoch(0));

    int id;
    QString email;
    QString password;
    QString name;
    QString description;
    QDateTime date;
};

#endif // SGDEVELOPER_H
