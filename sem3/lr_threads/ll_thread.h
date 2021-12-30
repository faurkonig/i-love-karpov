#ifndef LL_THREAD_H
#define LL_THREAD_H
#include <QThread>
#include <QMutex>


class ll_thread : public QThread
{
    Q_OBJECT
public:
    ll_thread(int number = 0);

    void run();

    QMutex * mtx; //мьютекс

    int stch; //счетчик
    int com; //код команды

    int getNum();

signals:
    void status_upd();

private:
    int num;
};

#endif // LL_THREAD_H
