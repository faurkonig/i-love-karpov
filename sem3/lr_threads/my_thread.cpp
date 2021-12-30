#include "my_thread.h"

my_thread::my_thread() //конструктор
{
    stch=0;

}

void my_thread::run() //выполнение потока
{
    int r=stch;
    stch=-1;
    msleep(r);
    stch=r;
}
//поток ждет случайное время.
//Возвращает значение -1 в процессе работы или время ожидания, после выполнения
