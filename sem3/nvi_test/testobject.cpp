#include "testobject.h"

TestObject::TestObject(int testTime, int correctAnswersNeeded, QVector<TestExercise> exercises)
{
    this->testTimeInSeconds = testTime;
    this->correctAnswersNeeded = correctAnswersNeeded;
    this->exercises = exercises;
}

int TestObject::getTimeInSeconds()
{
    return testTimeInSeconds;
}

bool TestObject::isPassed(int correctCount)
{
    return correctCount >= correctAnswersNeeded;
}

QVector<TestExercise> *TestObject::getExercises()
{
    return &exercises;
}

TestExercise *TestObject::operator[](int index)
{
    return &exercises[index];
}
