#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>

namespace Ui {
class FriendItem;
}

/// Виджет для отображения друга в списке
class FriendItem : public QWidget
{
    Q_OBJECT

public:
    /// Конструктор
    explicit FriendItem(int friendship, QString userName, QWidget *parent = nullptr);
    /// Деструктор
    ~FriendItem();

signals:
    /// Сигнал нажатие на кнопку удаления из друзей
    void onDeleteButtonPressed(int friendshipId);

private slots:
    /// Обработчик нажатие на кнопку удаления из друзей.
    /// Вызывает сигнал [onDeleteButtonPressed]
    void on_deleteButton_clicked();

private:
    Ui::FriendItem *ui;

    /// ID объекта дружбы, чтобы его передовать в сигнал [onDeleteButtonPressed]
    int friendshipId;
};

#endif // FRIENDITEM_H
