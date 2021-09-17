#include "testobject.h"

/// Конструктор, требует количество времени на тест и список вопросов
TestObject::TestObject(int testTime, QVector<TestExercise> exercises)
{
    this->testTimeInSeconds = testTime;
    this->exercises = exercises;
}


/// Возвращает количество времени на тест в секундах
int TestObject::getTimeInSeconds()
{
    return testTimeInSeconds;
}

/// Возвращает количество заданий
int TestObject::getExercisesCount()
{
    return exercises.count();
}

/// Возвращает указатель на список вопросов
QVector<TestExercise> *TestObject::getExercises()
{
    return &exercises;
}

/// Перегрузка оператора, которая возвращает задание по передаваемому индексу
TestExercise *TestObject::operator[](int index)
{
    return &exercises[index];
}
