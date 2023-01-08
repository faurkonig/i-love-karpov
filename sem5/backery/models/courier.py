import random

from enum import Enum

from helpers.names import getRandomName
import constants


class Courier:
    # Сумма размеров заказов (хранится в переменной чтобы не считать)
    collected = 0
    # Список взятых заказов
    orderList: list[int]
    maximumStorage = 5

    deliveryTimeout = 0

    completedCount = 0

    def __init__(self, id: str, name: str):
        self.id = id
        self.name = name
        self.status = CourierState.relaxed
        self.orderList = []

    def giveOrder(self, orderIndex: int, orderSize: int):
        # Не берём заказ, если он уже не помещается
        if self.collected + orderSize > self.maximumStorage:
            return False
        # Если же заказ помещается, берём его
        self.collected += orderSize
        self.orderList.append(orderIndex)
        return True

    def startDelivery(self):
        self.status = CourierState.delivering
        self.deliveryTimeout = newDeliveryTimeout(len(self.orderList))

    def processDelivery(self):
        if self.status is not CourierState.delivering:
            return None
        self.deliveryTimeout -= 1
        if self.deliveryTimeout <= 0:
            return self.orderList[:]

    def completeDelivery(self):
        self.completedCount += self.collected
        self.orderList.clear()
        self.collected = 0
        self.status = CourierState.relaxed

    def statusText(self):
        match self.status:
            case CourierState.relaxed:
                return 'Отдыхает'
            case CourierState.delivering:
                return 'Доставляет, {} сек. ({} заказа на {} булочек)'.format(
                    self.deliveryTimeout, len(self.orderList), self.collected)


class CourierState(Enum):
    relaxed = 0
    delivering = 1


def createNew():
    return Courier(str(random.randint(1000, 9999)),
                   getRandomName())


def newDeliveryTimeout(ordersCount: int):
    return constants.deliveryTimeout + ordersCount * constants.deliveryDeltaTimeout
