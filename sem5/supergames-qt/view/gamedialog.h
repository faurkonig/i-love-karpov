#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include "utils/databasecontainer.h"
#include <QDialog>

namespace Ui {
class GameDialog;
}

class GameDialog : public QDialog, private DatabaseContainer
{
    Q_OBJECT

public:
    /// Конструктор
    explicit GameDialog(QSqlDatabase *newDb, int id, int currentUser, QWidget *parent = nullptr);
    /// Деструктор
    ~GameDialog();

private slots:
    /// Обработчик нажатия на кнопку покупки/добавления игры
    void on_buyButton_clicked();

private:
    Ui::GameDialog *ui;

    /// ID игры, которую надо показать
    int gameId;
    /// ID пользователя,
    int userId;

    /// Цена игры (нужен для обновления на кнопке)
    int gamePrice;

    /// Флаг, отвечающий за присутствие игры в коллекции
    bool inCollection;
    /// Флаг, отвечающий за присутствие игры в корзине
    bool inCart;

    /// Метод для обновления данных внутри диалога
    void updateData();

    /// Метод для обновления кнопки покупки.
    /// Вызывается чаще, чем [updateData], так как данные даже без запроса надо
    /// обновлять по нажатию на кнопку.
    /// Поэтому метод и вынесен отдельно
    void updateBuyButton();
};

#endif // GAMEDIALOG_H
