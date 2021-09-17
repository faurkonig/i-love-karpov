#include "testexercise.h"

/// Конструктор, требует текст вопроса и список ответов
TestExercise::TestExercise(QString exerciseText, QVector<ExerciseAnswer> answers)
{
    this->exerciseText = exerciseText;
    this->answers = answers;
}


/// Возвращает текст вопроса
QString TestExercise::getText()
{
    return exerciseText;
}

/// Возвращает указатель на список вопросов
QVector<ExerciseAnswer> *TestExercise::getAnswers()
{
    return &answers;
}

/// Возвращает индекс первого правильного ответа, если правильного ответа нет, возвращает -1
int TestExercise::getCorrectAnswerIndex()
{
    for (int i = 0; i < answers.count(); i++)
    {
        if (answers[i].isCorrect())
            return i;
    }
    return -1;
}
