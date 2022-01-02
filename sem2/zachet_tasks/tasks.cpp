#include "tasks.h"

Tasks::Tasks()
{

}

void Tasks::addTask(QString title)
{
    Task newTask;
    newTask.title = title;
    tasks.append(newTask);
}

void Tasks::removeTask(int index)
{
    tasks.removeAt(index);
}

QString Tasks::getTitle(int index)
{
    return tasks[index].title;
}

bool Tasks::getCompleted(int index)
{
    return tasks[index].isCompleted;
}

void Tasks::setCompleted(int index, bool value)
{
    tasks[index].isCompleted = value;
}

int Tasks::getCount()
{
    return tasks.length();
}
