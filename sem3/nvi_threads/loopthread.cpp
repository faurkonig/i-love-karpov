#include "loopthread.h"


/// Конструктор
LoopThread::LoopThread(int num, QObject *parent) : QThread(parent)
{
    number = num;
    mtx = new QMutex();
    lock();
}

/// Деструктор
LoopThread::~LoopThread()
{
    delete mtx;
}


/// Перезаписанный метод от QThread, выполняется при запуске потока
void LoopThread::run()
{
    do
    {
        lock();

        // Выбор случайного времени исполнения
        // Сид зависит от номера для того, чтобы при запуске нескольких потоков одновременно,
        // время работы потока было разное
        if (number == 0)
            srand(static_cast<unsigned int>(clock()));
        else
            srand(static_cast<unsigned int>(clock() * number));

        int r{0};
        switch (command) {
        case 1: // Была команда "работать"
            r = rand() % 8000;
            break;
        case 2: // Была команда "умирать"
            r = rand() % 4000;
            break;
        }

        // Ожидание случайного времени
        msleep(static_cast<unsigned long>(r));

        counter = r;
        switch (command)
        {
        case 1: // Была команда "работать"
            command = 3;
            break;
        case 2: // Была команда "умирать"
            command = 4;
            break;
        }

        emit statusUpdate();
    }
    while (command != 4);
}


/// Блокировка мьютексе
void LoopThread::lock()
{
    mtx->lock();
}

/// Разблокировка мьютекса
void LoopThread::unlock()
{
    mtx->unlock();
}


/// Геттер counter
int LoopThread::getCounter()
{
    return counter;
}

/// Сеттер command
void LoopThread::setCommand(int value)
{
    command = value;
}

/// Геттер command
int LoopThread::getCommand()
{
    return command;
}

/// Геттер number
int LoopThread::getNumber()
{
    return number;
}
