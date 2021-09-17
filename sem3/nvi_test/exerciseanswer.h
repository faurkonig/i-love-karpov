#ifndef EXERCISEANSWER_H
#define EXERCISEANSWER_H

#include <QString>


/// Класс содержащий информацию об единственном ответе
class ExerciseAnswer
{
public:
    ExerciseAnswer(QString text, bool correct);

    QString getText();
    bool isCorrect();

private:
    QString text;
    bool correct;
};

#endif // EXERCISEANSWER_H
