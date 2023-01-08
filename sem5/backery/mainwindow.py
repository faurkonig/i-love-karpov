import sys

import PySide6.QtCore
from PySide6.QtWidgets import QApplication, QMainWindow, QTableWidget, QTableWidgetItem
from PySide6.QtCore import Slot

# pyside6-uic form.ui -o ui_form.py
from ui_form import Ui_MainWindow

from initialwindow import InitialWindow, InitialConfig
from helpers.global_timer import GlobalTimer
import models.order
import models.backer
import models.courier
import models.storage


class MainWindow(QMainWindow):
    orders: list[models.order.Order] = []
    orderTimeout = models.order.newOrderTimeout()

    backers: list[models.backer.Backer] = []
    bunTimeout = models.backer.newBunTimeout()

    couriers: list[models.courier.Courier] = []

    storage = models.storage.BackeryStorage()

    money = 0

    def __init__(self, config: InitialConfig, parent=None):
        super().__init__(parent)

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.ui.pauseButton.clicked.connect(self._gamePause)

        self.money = config.calculateMoney()
        self.storage.maximumStorage = config.storage
        for _ in range(config.backers):
            self.addBacker()
        for _ in range(config.couriers):
            self.addCourier()

        self._updateStorage()
        self._updateMoney()

        # Глобальный счётчик времени
        self._globalTimer = GlobalTimer(self)
        self._globalTimer.onTick.connect(self._timerTick)

    @Slot()
    def _timerTick(self):
        formatedTime = self._globalTimer.currentTime.toString('hh:mm:ss')
        print('Timer ticking:', formatedTime)
        self.ui.timeLabel.setText(formatedTime)

        self._processOrders()
        self._processCouriers()
        self._processBackers()

    @Slot()
    def _gamePause(self):
        self._globalTimer.isPaused = self.ui.pauseButton.isChecked()
        print('Game paused:', self._globalTimer.isPaused)

    def _processOrders(self):
        self.orderTimeout -= 1
        if self.orderTimeout <= 0:
            # Если пришло время создавать новый заказ
            self.orderTimeout = models.order.newOrderTimeout()
            self.addOrder()

        # Обновляем текст с временем до следующего заказа
        self.ui.orderTimeLabel.setText(
            'Секунд до следующего заказа: <b>{}</b>'.format(self.orderTimeout))

    def addOrder(self):
        newOrder = models.order.createNew()
        print('Added order:', newOrder.id)
        # Добавляем заказ в начало списка
        newOrder.orderTime = self._globalTimer.currentTime
        self.orders.insert(0, newOrder)
        # Обновляем таблицу заказов
        self.ui.ordersTable.setRowCount(len(self.orders))
        for i in range(len(self.orders)):
            self._updateOrder(i)

    def _updateOrder(self, index: int):
        table = self.ui.ordersTable
        order = self.orders[index]

        _initTableRow(table, index)
        table.item(index, 0).setText(order.id)
        table.item(index, 1).setText(order.customerName)
        table.item(index, 2).setText(str(order.orderSize))
        if order.orderTime is not None:
            table.item(index, 3).setText(
                order.orderTime.toString('hh:mm:ss')
            )
        if order.readyTime is not None:
            table.item(index, 4).setText(
                order.readyTime.toString('hh:mm:ss')
            )
        table.item(index, 5).setText(order.statusText())

    def _processBackers(self):
        if not self.storage.hasSpace():
            # Если на складе нет места, не делаем булочки
            self.ui.backerTimeLabel.setText(
                'Секунд до изготовления булочек: <b>-</b>')
            return
        self.bunTimeout -= 1
        if self.bunTimeout <= 0:
            self.bunTimeout = models.backer.newBunTimeout()
            self.addBuns()

        self.ui.backerTimeLabel.setText(
            'Секунд до изготовления булочек: <b>{}</b>'.format(self.bunTimeout))

    def addBuns(self):
        # Пытаемся испечь столько булочек, сколько можем
        bunBacked = self.storage.addBun(len(self.backers))
        print('Added buns:', bunBacked)
        self._updateStorage()
        for i in range(bunBacked):
            # Обновляем информацию для тех, кто смог испечь
            self.backers[i].completedCount += 1
            self._updateBacker(i)

    def addBacker(self):
        self.backers.append(models.backer.createNew())
        self.ui.backersTable.setRowCount(len(self.backers))
        self._updateBacker(len(self.backers) - 1)

    def _updateBacker(self, index: int):
        table = self.ui.backersTable
        backer = self.backers[index]

        _initTableRow(table, index)
        table.item(index, 0).setText(backer.id)
        table.item(index, 1).setText(backer.name)
        table.item(index, 2).setText(str(backer.completedCount))

    def _processCouriers(self):
        ordersBanList: set[int] = set()
        bunsChanged = False

        for courierIndex in range(len(self.couriers)):
            courier = self.couriers[courierIndex]
            # Сначала продолжаем доставку для тех, кто уже начал
            delivery = courier.processDelivery()
            if delivery is not None:
                for orderIndex in delivery:
                    print('Courier', courier.id, 'delivered orders',
                          delivery, 'with', courier.collected, 'buns')
                    self.money += courier.collected
                    self._updateMoney()

                    self.orders[orderIndex].complete(
                        self._globalTimer.currentTime)
                    courier.completeDelivery()
                    ordersBanList.add(orderIndex)
                    self._updateOrder(orderIndex)

            # Потом заполняем багажники тем, кто отдыхает
            print('courier cycle')
            if courier.status is models.courier.CourierState.relaxed:
                tookOrders = False  # Флаг указывающий на то, что этот курьер взял хотя бы один заказ
                for orderIndex in range(len(self.orders)):
                    # Пропускаем то, что уже взяли
                    if orderIndex in ordersBanList:
                        continue
                    order = self.orders[orderIndex]
                    # Пропускаем то, что взяли раньше
                    if order.status is not models.order.OrderState.created:
                        ordersBanList.add(orderIndex)
                        continue
                    # Пропускаем то, на что не хватит булочек со склада
                    if order.orderSize > self.storage.collected:
                        ordersBanList.add(orderIndex)
                        continue
                    # Пробуем выдать заказ курьеру
                    print('try to give order')
                    tookThisOrder = courier.giveOrder(
                        orderIndex, order.orderSize)
                    if tookThisOrder:
                        # Если заказ поместился у этого курьера
                        print('Courier', courier.id, 'took order', order.id,
                              '| His inventory:', courier.collected, '/', courier.maximumStorage)
                        tookOrders = True
                        bunsChanged = True
                        ordersBanList.add(orderIndex)
                        self.storage.takeBun(order.orderSize)
                        order.status = models.order.OrderState.inProcess
                        self._updateOrder(orderIndex)
                # Если курьер взял хотя бы 1 заказ, он начинает доставку
                if tookOrders:
                    courier.startDelivery()

            self._updateCourier(courierIndex)

        # Если есть хотя бы один курьер, который взял заказ
        if bunsChanged:
            self._updateStorage()

    def addCourier(self):
        self.couriers.append(models.courier.createNew())
        self.ui.courierTable.setRowCount(len(self.couriers))
        self._updateCourier(len(self.couriers) - 1)

    def _updateCourier(self, index: int):
        table = self.ui.courierTable
        courier = self.couriers[index]

        _initTableRow(table, index)
        table.item(index, 0).setText(courier.id)
        table.item(index, 1).setText(courier.name)
        table.item(index, 2).setText(str(courier.completedCount))
        table.item(index, 3).setText(courier.statusText())

    def _updateStorage(self):
        self.ui.storageStatsLabel.setText('На складе <b>{}</b> из <b>{}</b> булочек'
                                          .format(self.storage.collected,
                                                  self.storage.maximumStorage))

    def _updateMoney(self):
        orderCount = 0
        for o in self.orders:
            if o.status is models.order.OrderState.ready:
                orderCount += 1
        deliveredCount = 0
        for c in self.couriers:
            deliveredCount += c.completedCount
        self.ui.orderStatsLabel.setText('Всего выполнено <b>{}</b> заказов на сумму <b>{}</b> $'
                                        .format(orderCount, deliveredCount))
        self.ui.moneyLabel.setText(str(self.money) + ' $')


def _initTableRow(table: QTableWidget, y: int):
    for x in range(table.columnCount()):
        if table.item(y, x) is None:
            item = QTableWidgetItem()
            item.setFlags(item.flags() ^
                          (PySide6.QtCore.Qt.ItemFlag.ItemIsEditable |
                           PySide6.QtCore.Qt.ItemFlag.ItemIsSelectable |
                           PySide6.QtCore.Qt.ItemFlag.ItemIsUserCheckable))
            table.setItem(y, x, item)


if __name__ == "__main__":
    app = QApplication(sys.argv)

    config = InitialConfig()
    welcome = InitialWindow(config)
    welcome.exec()

    if config.isAccepted:
        print('Game accepted')
        widget = MainWindow(config)
        widget.show()
        sys.exit(app.exec())
    else:
        print('Game aborted')
        sys.exit(0)
