#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include <QVector>

#include "testexercise.h"


/// Класс, отвечающий за хранение всех вопросов, времени на тест
class TestObject
{
public:
    /// Конструктор, требует количество времени на тест и список вопросов
    TestObject(int testTime, int correctAnswersNeeded, QVector<TestExercise> exercises);

    /// Возвращает количество времени на тест в секундах
    int getTimeInSeconds();
    /// Возвращает количество правильных ответов, которое было бы достаточно для зачёта
    bool isPassed(int correctCount);
    /// Возвращает указатель на список вопросов
    QVector<TestExercise> *getExercises();

    TestExercise *operator[](int index);

private:
    int testTimeInSeconds;
    int correctAnswersNeeded;
    QVector<TestExercise> exercises;
};

#endif // TESTOBJECT_H
