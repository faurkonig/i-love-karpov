#ifndef MY_THREAD_H
#define MY_THREAD_H
#include <QThread>


class my_thread : public QThread
{
public:
    my_thread();
    void run();
    int stch;

};

#endif // MY_THREAD_H
