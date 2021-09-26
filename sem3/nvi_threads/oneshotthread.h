#ifndef ONESHOTTHREAD_H
#define ONESHOTTHREAD_H

#include <QThread>

class OneshotThread : public QThread
{
public:
    explicit OneshotThread(QObject *parent = nullptr);

    void run();

    void setCounter(int value);
    int getCounter();

private:
    int counter{0};
};

#endif // ONESHOTTHREAD_H
