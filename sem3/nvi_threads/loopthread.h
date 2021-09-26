#ifndef LOOPTHREAD_H
#define LOOPTHREAD_H

#include <QThread>
#include <QMutex>

class LoopThread : public QThread
{
    Q_OBJECT

public:
    explicit LoopThread(int num = 0, QObject *parent = nullptr);
    ~LoopThread();

    void run();

    void lock();
    void unlock();

    int getCounter();
    void setCommand(int value);
    int getCommand();

    int getNumber();

private:
    QMutex *mtx;

    int counter{0};
    int command{0};
    int number;

signals:
    void statusUpdate();
};

#endif // LOOPTHREAD_H
