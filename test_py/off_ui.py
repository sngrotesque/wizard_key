# 导入PyQt5模块
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QPushButton
from PyQt5.QtCore import QTimer, QTime
from PyQt5.QtGui import QFont
import subprocess
import win32api
import sys

# 定义一个窗口类，继承自QWidget
class MyWindow(QWidget):
    # 初始化方法
    def __init__(self):
        # 调用父类的初始化方法
        super().__init__()
        # 设置窗口标题和大小
        self.setWindowTitle('下班倒计时')
        self.resize(234, 70)
        # 设置字体
        self.font = QFont("Ubuntu Mono derivative Powerline", 14, QFont.Normal)
        # 创建一个标签组件，用来显示剩余时间
        self.label = QLabel(self)
        self.label.setFont(self.font)
        self.label.move(28, 2)
        self.label.resize(256, 40)
        # 创建一个按钮组件，用来手动关机
        self.button = QPushButton(self)
        self.button.setText('立即关机')
        self.button.move(77, 40)
        # 连接按钮的clicked信号到shutdown槽函数
        self.button.clicked.connect(self.shutdown)
        # 创建一个定时器对象，用来每秒更新剩余时间
        self.timer = QTimer(self)
        # 连接定时器的timeout信号到update_time槽函数
        self.timer.timeout.connect(self.update_time)
        # 启动定时器，设置间隔为1000毫秒（即1秒）
        self.timer.start(1000)

    # 定义一个更新剩余时间的槽函数
    def update_time(self):
        # 获取当前时间和下班时间
        now_time = QTime.currentTime()
        off_time = QTime(17, 27, 0)
        # 如果当前时间大于等于下班时间，调用shutdown方法
        if now_time >= off_time:
            self.shutdown()
        else:
            # 否则，计算剩余时间，并显示在标签上
            remain_time = now_time.secsTo(off_time)
            self.label.setText(f'距离下班还有{remain_time:>5d}秒')

    # 定义一个关机的槽函数
    def shutdown(self):
        # 停止定时器
        self.timer.stop()
        # 调用系统命令，强制关机
        # subprocess.call('shutdown /f /p', shell = True)
        win32api.MessageBox(0, '关机测试', '关机测试')

# 创建一个应用对象
app = QApplication(sys.argv)
# 创建一个窗口对象
window = MyWindow()
# 显示窗口
window.show()
# 进入应用的事件循环
sys.exit(app.exec_())
