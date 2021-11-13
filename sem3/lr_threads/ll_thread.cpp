#include "ll_thread.h"

ll_thread::ll_thread(int number) // конструктор
{
    num = number;

    mtx = new QMutex;
    com=0;
    mtx->lock();
}

void ll_thread::run() //выполнение потока
{
    do
    {
        mtx->lock();
        int r;
        srand(clock() * (num + 1));
        switch (com)
        {
        case 1: //работать
            r=rand()%8000;
            break;
        case 2: //завершение
            r=rand()%4000;
            break;
        default:
            r=0;
            break;
        }
        msleep(r);
        stch=r;
        switch (com)
        {
        case 1:
            com=3;
            break;
        case 2:
            com=4;
            break;
        default:
            com=0;
            break;
        }

        emit status_upd();
    }
    while (com!=4);
}

int ll_thread::getNum() //возвращение номера потока
{
    return num;
}
