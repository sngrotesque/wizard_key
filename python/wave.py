import matplotlib.pyplot as plt
from scipy.io.wavfile import write
from scipy import signal
import numpy as np

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
