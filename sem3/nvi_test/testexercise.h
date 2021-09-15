#ifndef TESTEXERCISE_H
#define TESTEXERCISE_H

#include <QString>
#include <QVector>

#include "exerciseanswer.h"


/// Класс, отвечающий за единственный вопрос в тесте, содержит текст вопроса и его ответы
class TestExercise
{
public:
    /// Конструктор, требует текст вопроса и список ответов
    TestExercise(QString exerciseText, QVector<ExerciseAnswer> answers);

    /// Возвращает текст вопроса
    QString getText();
    /// Возвращает указатель на список вопросов
    QVector<ExerciseAnswer> *getAnswers();
    /// Возвращает индекс первого правильного ответа, если правильного ответа нет, возвращает -1
    int getCorrectAnswerIndex();

private:
    QString exerciseText;
    QVector<ExerciseAnswer> answers;
};

#endif // TESTEXERCISE_H
