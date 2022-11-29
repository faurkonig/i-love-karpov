#ifndef STOREWINDOW_H
#define STOREWINDOW_H

#include "utils/databasecontainer.h"
#include "sguser.h"
#include "components/gameitem.h"
#include <QMainWindow>

namespace Ui {
/// Окно магазина (у пользователя)
class StoreWindow;
}

class StoreWindow : public QMainWindow, private DatabaseContainer
{
    Q_OBJECT

public:
    /// Конструктор
    explicit StoreWindow(SgUser user, QSqlDatabase *newDb, QWidget *parent = nullptr);
    /// Деструктор
    ~StoreWindow();

private slots:
    /// Обработчик нажатия на кнопку обновления данных
    void on_updateButton_clicked();

    /// Обработчик нажатия на кнопку профиля
    void on_profileButton_clicked();

    /// Обработчик нажатия на кнопку "Об игре" у какой-то конкретной игры
    void openGame(int gameId);

    /// Обработчик нажатия на кнопку коллекции
    void on_collectionButton_clicked();

    /// Обработчик нажатия на кнопку корзины
    void on_cartButton_clicked();

private:
    Ui::StoreWindow *ui;

    SgUser user;

    QList<GameItem *> gameItems;

    /// Метод для обновления списка игр
    void updateGames();
};

#endif // STOREWINDOW_H
