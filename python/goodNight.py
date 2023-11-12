from PyQt5.QtWidgets import QApplication, QWidget, QLCDNumber, QVBoxLayout
from PyQt5.QtCore import QTimer, QTime
import subprocess
import sys

class Countdown(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('倒计时')
        self.setStyleSheet('background-color: #222222;')
        self.showFullScreen()
        
        self.lcd = QLCDNumber() # 创建一个QLCDNumber对象，用于显示倒计时
        self.lcd.setDigitCount(8) # 设置显示的位数为8
        self.lcd.setMode(QLCDNumber.Dec) # 设置显示的模式为十进制
        self.lcd.setSegmentStyle(QLCDNumber.Flat) # 设置显示的样式为平面样式
        
        self.timer = QTimer() # 创建一个QTimer对象，用于定时发出信号
        self.timer.timeout.connect(self.updateTime) # 将定时器的timeout信号连接到updateTime槽函数
        self.time = QTime(0, 30, 0) # 创建一个QTime对象，用于表示倒计时的初始值
        self.lcd.display(self.time.toString('HH:mm:ss')) # 使用toString方法将时间格式化为HH:mm:ss形式，并显示在QLCDNumber上
        
        layout = QVBoxLayout() # 创建一个垂直布局
        layout.addWidget(self.lcd) # 将QLCDNumber添加到布局中
        self.setLayout(layout) # 设置窗口的布局
        self.timer.start(1000) # 启动定时器，设置时间间隔为1秒

    def updateTime(self):
        if self.time == QTime(0, 0, 0): # 判断时间是否已经到达00:00:00
            self.timer.stop() # 停止定时器
            self.close() # 关闭窗口
            subprocess.call('shutdown /f /s /t 0')
        else:
            self.time = self.time.addSecs(-1) # 将时间减少1秒
            self.lcd.display(self.time.toString('HH:mm:ss')) # 将时间格式化并显示在QLCDNumber上

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = Countdown()
    window.show()
    sys.exit(app.exec_())
