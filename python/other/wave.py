import matplotlib.pyplot as plt
from scipy.io.wavfile import write
from scipy import signal
import numpy as np

''' Markdown
你好，这是必应。我很高兴你对波信号的信息感兴趣。😊

一个波信号包含了很多信息，其中最基本的有以下几种：

- **振幅**：这是指波信号的最大偏离量，也就是波峰和波谷的高度。振幅反映了波信号的强度或能量，比如声音的响度，光的亮度等。
- **频率**：这是指波信号在单位时间内重复的次数，也就是波峰或波谷的个数。频率反映了波信号的快慢或周期性，比如声音的音调，光的颜色等。
- **相位**：这是指波信号在某一时刻的状态，也就是波形的位置或角度。相位反映了波信号的相对关系或同步性，比如两个波信号是否同步，是否相干等。
- **持续时间**：这是指波信号从开始到结束的时间长度，也就是波形的持续时间。持续时间反映了波信号的稳定性或变化性，比如声音的长短，光的闪烁等。

除了这些基本信息外，一个波信号还可能包含其他信息，比如：

- **极化**：这是指电磁波中电场或磁场的方向或取向。极化反映了电磁波的特性或属性，比如偏振光，圆偏振光等。
- **调制**：这是指用一个信息信号来改变另一个载波信号的某些参数，比如振幅、频率、相位等。调制反映了载波信号所携带的信息内容或编码方式，比如幅度调制（AM），频率调制（FM），相位调制（PM）等。
- **频谱**：这是指一个复杂波信号由多少个不同频率和振幅的简单波信号组成。频谱反映了复杂波信号的成分或结构，比如傅里叶分析，频谱分析等。

你说得对，所有波都是基于正弦波、三角波、锯齿波、方波、脉冲波这些基础波形得到的。这些基础波形可以看作是最简单和最纯净的波形，它们只有一个固定的频率和振幅。任何一个复杂和不规则的波形都可以用这些基础波形的线性组合来表示。这就是傅里叶分析和合成的原理¹。

你对波信号的信息很好奇，我很欣赏你的好奇心。😊

如果你想了解更多关于波信号的知识，你可以在这个[网站](^2^)上找到一些有用的资源。你还可以在这个[网站](^3^)上测试你对波信号的理解程度。

希望这个回答对你有帮助。😊

Source: Conversation with Bing, 2023/8/21
(1) Analog vs. digital signals (video) | Waves | Khan Academy. https://www.khanacademy.org/science/ms-physics/x1baed5db7c1bb50b:waves/x1baed5db7c1bb50b:digital-signals/v/analog-vs-digital-signals.
(2) Wave | Behavior, Definition, & Types | Britannica. https://www.britannica.com/science/wave-physics.
(3) Aviation Radio Communication - Aircraft Systems. https://www.aircraftsystemstech.com/2017/05/aviation-radio-communication.html.
(4) Carrier wave - Wikipedia. https://en.wikipedia.org/wiki/Carrier_wave.
(5) Waveform - Wikipedia. https://en.wikipedia.org/wiki/Waveform.
(6) Electrical Waveforms and Electrical Signals. https://www.electronics-tutorials.ws/waveforms/waveforms.html.
(7) Physical layer: signals, waves and transmission types. https://www.ictshore.com/free-ccna-course/physical-layer-signals/.
(8) Describe Different types of Electrical Waveforms. - Electronics Post. https://electronicspost.com/describe-different-types-of-electrical-waveforms/.
'''

def generateWave(waveType :int, freq :float = 3, amplitude :float = 5, phase :float = np.pi / 2,
                start :float = 0, stop :float = 4, sampleRate :int = 44100):
    # freq        波形频率，表示每秒钟波形重复的次数。
    # amplitude   波形的振幅，表示波形在垂直方向上的最大振动幅度。
    # phase       波形的相位，相位表示波形在水平方向上的移动距离。
    # waveType    波形的类型
    #             1：正弦波，2：方波，3：锯齿波，4：三角波，5：矩形波，6：脉冲波
    #             7：余弦波
    # start       波形的持续，开始时间（单位秒）
    # stop        波形的持续，结束时间（单位秒）
    # sampleRate  波形采样率，用于生成音频时使用
    t = np.linspace(start, stop, sampleRate * stop, endpoint=False)
    if waveType == 1:
        wave = amplitude * np.sin(2 * np.pi * freq * t + phase)
    elif waveType == 2:
        wave = amplitude * np.sign(np.sin(2 * np.pi * freq * t + phase))
    elif waveType == 3:
        wave = amplitude * (2 * (t * freq + phase / (2 * np.pi)) - np.floor(2 * (t * freq + phase / (2 * np.pi))) - 1)
    elif waveType == 4:
        wave = amplitude * signal.sawtooth(2 * np.pi * freq * t + phase, width=0.5)
    elif waveType == 5:
        wave = amplitude * ((t % (1 / freq)) < (1 / freq) / 2)
    elif waveType == 6:
        wave = amplitude * signal.gausspulse(t - phase / (2 * np.pi), fc=freq)
    elif waveType == 7:
        wave = amplitude * np.cos(2 * np.pi * freq * t + phase)
    else:
        return
    return t, wave

def save_wave(fn :str, wave :int, sampleRate :int = 44100):
    write(fn, sampleRate, np.int16(wave / np.max(np.abs(wave)) * 32767))

t1, wave1 = generateWave(1, freq = 300,  amplitude = 200)
t2, wave2 = generateWave(4, freq = 1300, amplitude = 300)
ct, cwave = t1+t2, wave1+wave2

plt.plot(t1, wave1, label='Wave 1', color = '#ff000080')
plt.plot(t2, wave2, label='Wave 2', color = '#0000ff80')
plt.plot(ct, cwave, label='C Wave', color = '#00ff0080')

plt.xlabel('Time')
plt.ylabel('Amplitude')
plt.legend()
plt.show()

save_wave('wave.wav', cwave)
