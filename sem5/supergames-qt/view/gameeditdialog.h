#ifndef GAMEEDITDIALOG_H
#define GAMEEDITDIALOG_H

#include "utils/databasecontainer.h"
#include <QDialog>

namespace Ui {
class GameEditDialog;
}

/// Диалог создания или редактирования игры
class GameEditDialog : public QDialog, private DatabaseContainer
{
    Q_OBJECT

public:
    /// Конструктор
    explicit GameEditDialog(bool isNew, int gameId, int devId, bool *isChanged, QSqlDatabase *newDb, QWidget *parent = nullptr);
    /// Деструктор
    ~GameEditDialog();

private slots:
    /// Обработчик нажатия на кнопку сохранения игры
    void on_saveButton_clicked();

    /// Обработчик нажатия на кнопку удаления игры
    void on_deleteButton_clicked();

    /// Обработчик нажатия на кнопку отмены
    void on_cancelButton_clicked();

    /// Обработчик закрытия диалога
    void reject();

private:
    Ui::GameEditDialog *ui;

    bool isGameNew;
    int gameId;
    int devId;

    bool *isSomethingChanged;

    // Последние сохранённые состояния (для проверки при закрытии)
    QString lastSavedName;
    QString lastSavedDescription;
    double lastSavedPrice;

    /// Метод для обновления содержимого диалога
    void updateContent();
};

#endif // GAMEEDITDIALOG_H
