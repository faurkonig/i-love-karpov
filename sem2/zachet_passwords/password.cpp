#include "password.h"

Password::Password()
{
    // Конструктор
}

void Password::setTitle(int index, QString val)
{
    // Присвоение заголовка
    titles[index] = val;
}

void Password::setPass(int index, QString val)
{
    // Присвоение пароля
    passes[index] = val;
}

QString Password::getTitle(int index)
{
    // Возвращение заголовка
    return titles[index];
}

QString Password::getPass(int index)
{
    // Возвращение пароля
    return passes[index];
}

void Password::addPassword(QString title, QString password)
{
    titles.append(title);
    passes.append(password);
}

void Password::removePassword(int index)
{
    titles.removeAt(index);
    passes.removeAt(index);
}

int Password::getCount()
{
    return titles.count();
}
