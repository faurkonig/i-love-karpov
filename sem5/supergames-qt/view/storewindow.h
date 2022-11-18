#ifndef STOREWINDOW_H
#define STOREWINDOW_H

#include "utils/databasecontainer.h"
#include "sguser.h"
#include "components/gameitem.h"
#include <QMainWindow>

namespace Ui {
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

    /// Пользователь, под которым мы вошли
    SgUser user;

    /// Список всех виджетов игр
    QList<GameItem *> gameItems;

    /// SQL запрос для получения списка всех игры и информации для их отображения
    const QString gamesQuerySql{
        "SELECT"
        " g.id, g.\"name\", g.description, g.price,"
        " (SELECT d.name AS developer"
        "  FROM public.developers d"
        "  WHERE g.developer = d.id) "
        "FROM public.games g "
        "ORDER BY g.\"date\""
    };

    /// Метод для обновления списка игр
    void updateGames();
};

#endif // STOREWINDOW_H
