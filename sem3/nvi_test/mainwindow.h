#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QRadioButton>
#include <QButtonGroup>
#include <QMessageBox>

#include "testobject.h"
#include "testexercise.h"
#include "exerciseanswer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

    void on_nextExerciseButton_clicked();

    void on_finishTestButton_clicked();

    void onTestTimerTimeout();

    void onAnswerChecked(int id);

    void on_closeButton_clicked();

private:
    Ui::MainWindow *ui;

    TestObject defaultTest {
        // Тест
        30,
        3,
        {
            // Вопросы
            {
                // 1 вопрос
                "Какого цвета небо?",
                {
                    // Ответы
                    { "Синее", true },
                    { "Красное", false },
                    { "Зелёное", false },
                    { "Жёлтое", false },
                }
            },
            {
                // 2 вопрос
                "Какого цвета трава (летом)?",
                {
                    // Ответы
                    { "Фиолетовая", false },
                    { "Зелёная", true },
                }
            },
            {
                // 3 вопрос
                "Кто автор этой программы?",
                {
                    // Ответы
                    { "Егор Летов", false },
                    { "Влад Нагорный", true },
                    { "Слава Мерлоу", false },
                    { "Владимир Путин", false },
                }
            },
            {
                // 4 вопрос
                "А теперь самый провокационный вопрос:\n"
                "11001100 в 2сс это ____ в 10сс. Какое число должно быть на месте пропуска?",
                {
                    // Ответы
                    { "0", false },
                    { "1", false },
                    { "99", false },
                    { "204", true },
                    { "78", false },
                    { "46", false },
                }
            },
            {
                // 5 вопрос
                "Что такое осень?",
                {
                    // Ответы
                    { "Это небо", false },
                    { "Это камни", false },
                    { "Это ветер", false },
                    { "Тёмная даль", true },
                }
            },
        }
    };

    int estimatedSeconds{0};
    QTimer *testTimer{nullptr};

    int currentExercise{0};
    int currentAnswer{-1};
    int correctCount{0};
    QButtonGroup *answersButtons;

    void clearAnswersButtons();
    void updateExerciseWidgets();

    void updateTimeLabels(bool useCurrent = true);
    QString formatNumberAsTime(int seconds);

    void finishTest();
};

#endif // MAINWINDOW_H
