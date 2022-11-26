#ifndef REVIEWDIALOG_H
#define REVIEWDIALOG_H

#include "utils/databasecontainer.h"
#include <QDialog>

namespace Ui {
class ReviewDialog;
}

class ReviewDialog : public QDialog, private DatabaseContainer
{
    Q_OBJECT

public:
    /// Конструктор
    /// Принимает ID пользователя и БД для выполнения запросов
    explicit ReviewDialog(int userId, int gameId, QSqlDatabase *newDb, QWidget *parent = nullptr);
    /// Деструктор
    ~ReviewDialog();

private slots:
    /// Обработчик нажатия на кнопку "Отмена"
    void on_cancelButton_clicked();

    /// Обработчик сохранения отзыва
    void on_saveButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::ReviewDialog *ui;

    bool isExists{false};
    int userId;
    int gameId;
    int reviewId{0};

    /// Метод, обновляющий состояние и данные в диалоге
    void updateContent();
    /// Метод, сбрасывающий виджеты в состояние, как если отзыва на игру нет
    void resetContent();
};

#endif // REVIEWDIALOG_H
