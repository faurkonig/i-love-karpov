#include "testexercise.h"

TestExercise::TestExercise(QString exerciseText, QVector<ExerciseAnswer> answers)
{
    this->exerciseText = exerciseText;
    this->answers = answers;
}

QString TestExercise::getText()
{
    return exerciseText;
}

QVector<ExerciseAnswer> *TestExercise::getAnswers()
{
    return &answers;
}

int TestExercise::getCorrectAnswerIndex()
{
    for (int i = 0; i < answers.count(); i++)
    {
        if (answers[i].isCorrect())
            return i;
    }
    return -1;
}
