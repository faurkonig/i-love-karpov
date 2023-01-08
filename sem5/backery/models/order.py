import random
import string

from PySide6.QtCore import QTime
from enum import Enum

from helpers.names import getRandomName
import constants


class Order:
    orderTime: QTime | None = None
    readyTime: QTime | None = None

    def __init__(self, id: str, customerName: str, size: int):
        self.id = id
        self.customerName = customerName
        self.orderSize = size
        self.status = OrderState.created

    def complete(self, time: QTime):
        self.readyTime = time
        self.status = OrderState.ready

    def statusText(self):
        match self.status:
            case OrderState.created:
                return 'Не готово'
            case OrderState.inProcess:
                return 'Обрабатывается'
            case OrderState.ready:
                return 'Готово!'


class OrderState(Enum):
    created = 0
    inProcess = 1
    ready = 2


def createNew():
    orderId = random.choice(string.ascii_letters).capitalize(
    ) + '-' + str(random.randint(1000, 9999))
    return Order(orderId,
                 getRandomName(),
                 random.randint(constants.minOrderSize, constants.maxOrderSize))


def newOrderTimeout():
    return random.randint(constants.minOrderTimeout, constants.maxOrderTimeout)
    # return random.randint(1, 2)
