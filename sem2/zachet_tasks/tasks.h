#ifndef TASKS_H
#define TASKS_H

#include <QVector>


struct Task
{
    QString title;
    bool isCompleted{false};
};

class Tasks
{
public:
    Tasks();

    void addTask(QString title);
    void removeTask(int index);
    QString getTitle(int index);
    bool getCompleted(int index);
    void setCompleted(int index, bool value);
    int getCount();

private:
    QVector<Task> tasks;
};

#endif // TASKS_H
