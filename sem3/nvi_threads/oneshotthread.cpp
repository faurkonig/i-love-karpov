#include "oneshotthread.h"


/// Конструктор
OneshotThread::OneshotThread(QObject *parent) : QThread(parent)
{

}


/// Перезаписанный метод от QThread, выполняется при запуске потока
void OneshotThread::run()
{
    // Поток ждёт случайное время
    // Возвращает значение -1 в процессе работы или во время ожидания, после выполнения
    int r = counter;
    counter = -1;
    msleep(static_cast<unsigned long>(r));
    counter = r;
}


/// Геттер counter
void OneshotThread::setCounter(int value)
{
    counter = value;
}

/// Сеттер counter
int OneshotThread::getCounter()
{
    return counter;
}
