from PySide6.QtCore import QObject, QTimer, QTime, QThread, Slot, Signal


class GlobalTimer(QObject):
    onTick = Signal()
    currentTime = QTime(0, 0, 0, 0)

    isPaused = False

    _timer = QTimer()
    _thread = QThread()

    def __init__(self, parent=None):
        super().__init__(parent)

        self._timer.setInterval(1000)
        self._timer.timeout.connect(self._timerTick)

        self._thread.started.connect(self._threadStart)
        self._thread.start()

    @Slot()
    def _threadStart(self):
        print('Global thread start')
        self._timer.start()

    @Slot()
    def _timerTick(self):
        if not self.isPaused:
            self.currentTime = self.currentTime.addSecs(1)
            self.onTick.emit()
