#include "exerciseanswer.h"


/// Конструктор, требует текст ответа и указание на то, верен этот ответа или нет
ExerciseAnswer::ExerciseAnswer(QString text, bool correct)
{
    this->text = text;
    this->correct = correct;
}


/// Возвращате текст ответа
QString ExerciseAnswer::getText()
{
    return text;
}

/// Возвращает указание на правильность ответа
bool ExerciseAnswer::isCorrect()
{
    return correct;
}
