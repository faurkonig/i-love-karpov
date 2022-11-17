#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QWidget>

namespace Ui {
class GameItem;
}

class GameItem : public QWidget
{
    Q_OBJECT

public:
    /// Конструктор
    explicit GameItem(
            int id = 0, QString name = "", QString description = "", QString developer = "", double price = 0,
            QWidget *parent = nullptr);
    /// Деструктор
    ~GameItem();

signals:
    /// Сигнал нажатия на кнопку "Об игре".
    /// Передаёт ID игры, на которой была нажата кнопку
    void onGameButtonPressed(int gameId);

private slots:
    /// Обработчик нажатия на кнопку "Об игре".
    /// Вызывает сигнал [onGameButtonPressed] с передачей поля [id]
    void on_buyButton_clicked();

private:
    Ui::GameItem *ui;

    /// ID игры в базе данных
    int id;
};

#endif // GAMEITEM_H
