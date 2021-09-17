#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include <QVector>

#include "testexercise.h"


/// Класс, отвечающий за хранение всех вопросов, времени на тест
class TestObject
{
public:
    TestObject(int testTime, QVector<TestExercise> exercises);

    int getTimeInSeconds();
    int getExercisesCount();
    QVector<TestExercise> *getExercises();
    TestExercise *operator[](int index);

private:
    int testTimeInSeconds;
    QVector<TestExercise> exercises;
};

#endif // TESTOBJECT_H
