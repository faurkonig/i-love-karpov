#ifndef DEVGAMEITEM_H
#define DEVGAMEITEM_H

#include <QWidget>

namespace Ui {
class DevGameItem;
}

class DevGameItem : public QWidget
{
    Q_OBJECT

public:
    /// Конструктор
    explicit DevGameItem(int id, QString name, QString description, QWidget *parent = nullptr);
    /// Деструктор
    ~DevGameItem();

signals:
    /// Сигнал о нажатии на основную кнопку игры
    void onOpenGameButtonPressed(int gameId);

private slots:
    /// Обработчик нажатия на кнопку "Открыть в магазине"
    void on_openButton_clicked();

private:
    Ui::DevGameItem *ui;

    int gameId;
};

#endif // DEVGAMEITEM_H
