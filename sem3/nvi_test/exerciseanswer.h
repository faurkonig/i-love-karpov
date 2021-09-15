#ifndef EXERCISEANSWER_H
#define EXERCISEANSWER_H

#include <QString>


/// Класс содержащий информацию об единственном ответе
class ExerciseAnswer
{
public:
    /// Конструктор, требует текст ответа и указание на то, верен этот ответа или нет
    ExerciseAnswer(QString text, bool correct);

    /// Возвращате текст ответа
    QString getText();
    /// Возвращает указание на правильность ответа
    bool isCorrect();

private:
    QString text;
    bool correct;
};

#endif // EXERCISEANSWER_H
