#include "exerciseanswer.h"

ExerciseAnswer::ExerciseAnswer(QString text, bool correct)
{
    this->text = text;
    this->correct = correct;
}

QString ExerciseAnswer::getText()
{
    return text;
}

bool ExerciseAnswer::isCorrect()
{
    return correct;
}
