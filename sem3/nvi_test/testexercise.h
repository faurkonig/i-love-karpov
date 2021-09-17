#ifndef TESTEXERCISE_H
#define TESTEXERCISE_H

#include <QString>
#include <QVector>

#include "exerciseanswer.h"


/// Класс, отвечающий за единственный вопрос в тесте, содержит текст вопроса и его ответы
class TestExercise
{
public:
    TestExercise(QString exerciseText, QVector<ExerciseAnswer> answers);

    QString getText();
    QVector<ExerciseAnswer> *getAnswers();
    int getCorrectAnswerIndex();

private:
    QString exerciseText;
    QVector<ExerciseAnswer> answers;
};

#endif // TESTEXERCISE_H
