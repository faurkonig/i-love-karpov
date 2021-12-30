#include "passwords.h"

/// Конструктор
Passwords::Passwords()
{

}

/// Присвоение заголовка
void Passwords::setTitle(int index, QString val)
{
    titles[index] = val;
}

/// Присвоение пароля
void Passwords::setPass(int index, QString val)
{
    passes[index] = val;
}

/// Возвращение заголовка
QString Passwords::getTitle(int index)
{
    return titles[index];
}

/// Возвращение пароля
QString Passwords::getPass(int index)
{
    return passes[index];
}

/// Добавление пароля
void Passwords::addPassword(QString title, QString password)
{
    titles.append(title);
    passes.append(password);
}

/// Удаление пароля
void Passwords::removePassword(int index)
{
    titles.removeAt(index);
    passes.removeAt(index);
}

/// Получение количества паролей
int Passwords::getCount()
{
    return titles.count();
}
