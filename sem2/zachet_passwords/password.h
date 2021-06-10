#ifndef PASSWORD_H
#define PASSWORD_H

#include <QString>
#include <QVector>

class Password
{
public:
    Password();

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

#endif // PASSWORD_H
