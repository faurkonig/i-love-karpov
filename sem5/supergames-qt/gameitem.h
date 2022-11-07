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
    explicit GameItem(
            int id = 0, QString name = "", QString description = "", QString developer = "", double price = 0,
            QWidget *parent = nullptr);
    ~GameItem();

signals:
    void onGameButtonPressed(int gameId);

private slots:
    void on_buyButton_clicked();

private:
    Ui::GameItem *ui;

    int id;
//    QString name;
//    QString description;
//    QString developer;
//    double price;
};

#endif // GAMEITEM_H
