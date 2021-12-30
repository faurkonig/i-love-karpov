#ifndef PASSWORDS_H
#define PASSWORDS_H

#include <QString>
#include <QVector>

class Passwords
{
public:
    Passwords();

    void setTitle(int index, QString val);
    void setPass(int index, QString val);
    QString getTitle(int index);
    QString getPass(int index);
    void addPassword(QString title, QString password);
    void removePassword(int index);
    int getCount();

private:
    QVector<QString> titles;
    QVector<QString> passes;
};

#endif // PASSWORDS_H
