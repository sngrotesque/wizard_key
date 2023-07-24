import time

class wmkcTime:
    def __init__(self):
        self.timer = None

    def timerBegin(self):
        self.timer = time.time()

    def timerEnd(self):
        self.timer = time.time() - self.timer

