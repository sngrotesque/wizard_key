CSPRNG和TRNG都有很多不同的实现方式，你可以根据你的需求和环境来选择合适的库或工具。我为你找了一些网上的资源，你可以参考一下：
CSPRNG是指一种伪随机数生成器，它具有密码学上的安全性，也就是说它产生的随机数序列很难被预测或重现1。CSPRNG通常基于一些复杂的算法或函数，比如哈希函数、流密码、块密码等1。CSPRNG有很多现成的库或工具，比如：
CSPRNG2是一个小型的C和C++库，它使用操作系统本身提供的CSPRNG接口，比如Windows下的BCryptGenRandom、CryptGenRandom或RtlGenRandom等。
CSPRNG D Library3是一个用于生成密码学随机数的D语言库，它也依赖于操作系统提供的接口。
TRNG是指一种真随机数生成器，它基于一些物理现象或设备来产生随机数，比如电子噪声、量子效应、硬件加速器等45。TRNG产生的随机数序列没有任何规律或模式，但是可能受到外界干扰或偏差影响45。TRNG也有很多现成的库或工具，比如：
TRNG4是一个现代化的C++伪随机数生成器库，它完全兼容C++11标准，并且支持多线程和分布式计算环境。
Tina’s Random Number Generator Library5是一个面向对象设计、易于使用并且速度优化过的伪随机数生成器库，它适用于共享内存和分布式内存计算机，并且可以在任何并行编程环境中使用。
我希望这些信息能对你有所帮助。如果你想了解更多关于这些库或工具的细节，请访问他们各自提供给我的链接。

https://en.wikipedia.org/wiki/Cryptographically_secure_pseudorandom_number_generator
https://github.com/Duthomhas/CSPRNG
https://github.com/JonathanWilbur/csprng-d
https://github.com/rabauke/trng4
https://www.numbercrunch.de/trng/
https://github.com/topics/trng
