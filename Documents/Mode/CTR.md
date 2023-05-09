一 什么是CTR模式

CTR模式全称CounTeR模式（计数器模式）。CTR模式是一种通过将逐次累加的计数器进行加密来生成密钥流的流密码。

![](https://img-blog.csdn.net/20180905192420867?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoZW5ncWl1bWluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

CTR模式中，每个分组对应一个逐次累加的计数器，并通过对计数器进行加密来生成密钥流。也就是说，最终的密文分组是通过将计数器加密而得到的比特序列，与明文分组进行XOR而得到的。

二 计数器的生成方法

每次加密时都会生成一个不同的值（nonce）作为计数器的初始值。当分组长度为128比特时，计数器的初始值可能如下图所示：

![](https://img-blog.csdn.net/20180905192456927?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoZW5ncWl1bWluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

加密过程中，计数器的值会产生如下变化：

![](https://img-blog.csdn.net/20180905192511977?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoZW5ncWl1bWluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

三 OFB模式与CTR模式的对比

![](https://img-blog.csdn.net/20180905192526326?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoZW5ncWl1bWluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

CTR模式和OFB模式都属于流密码。

OFB模式是将加密的输出反馈到输入，而CTR模式则是将计数器的值用作输入。

四 CTR模式的特点

CTR模式的加密和解密使用了完全相同的模式，因此在程序上实现上比较容易。

CTR模式中可以以任意顺序对分组进行加密和解密，因为在加密和解密时需要用到的“计数器”的值可以由nonce和分组序号直接计算出来。

能够以任意顺序处理分组，意味着能够实现并行计算。在支持并行计算的系统中，CTR模式的速度是非常快的。

五 错误与机密性

假设CTR模式的密文分组中有一个比特被反转了，则解密后明文分组中仅有与之对应的比特会被反转，这一错误不会放大。

主动攻击者可以通过反转密文分组中的某些比特，引起解密后明文中的相应比特也发生反转。

CTR模式中，如果对密钥流的一个分组进行加密后碰巧和加密前是相同的，那么这个分组之后的密钥流就会变成同一值的不断反复，这个在CTR中不存在这一问题。

一 什么是CTR模式

CTR模式全称CounTeR模式（计数器模式）。CTR模式是一种通过将逐次累加的计数器进行加密来生成密钥流的流密码。

![](https://img-blog.csdn.net/20180905192420867?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoZW5ncWl1bWluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

CTR模式中，每个分组对应一个逐次累加的计数器，并通过对计数器进行加密来生成密钥流。也就是说，最终的密文分组是通过将计数器加密而得到的比特序列，与明文分组进行XOR而得到的。

二 计数器的生成方法

每次加密时都会生成一个不同的值（nonce）作为计数器的初始值。当分组长度为128比特时，计数器的初始值可能如下图所示：

![](https://img-blog.csdn.net/20180905192456927?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoZW5ncWl1bWluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

加密过程中，计数器的值会产生如下变化：

![](https://img-blog.csdn.net/20180905192511977?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoZW5ncWl1bWluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

三 OFB模式与CTR模式的对比

![](https://img-blog.csdn.net/20180905192526326?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoZW5ncWl1bWluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

CTR模式和OFB模式都属于流密码。

OFB模式是将加密的输出反馈到输入，而CTR模式则是将计数器的值用作输入。

四 CTR模式的特点

CTR模式的加密和解密使用了完全相同的模式，因此在程序上实现上比较容易。

CTR模式中可以以任意顺序对分组进行加密和解密，因为在加密和解密时需要用到的“计数器”的值可以由nonce和分组序号直接计算出来。

能够以任意顺序处理分组，意味着能够实现并行计算。在支持并行计算的系统中，CTR模式的速度是非常快的。

五 错误与机密性

假设CTR模式的密文分组中有一个比特被反转了，则解密后明文分组中仅有与之对应的比特会被反转，这一错误不会放大。

主动攻击者可以通过反转密文分组中的某些比特，引起解密后明文中的相应比特也发生反转。

CTR模式中，如果对密钥流的一个分组进行加密后碰巧和加密前是相同的，那么这个分组之后的密钥流就会变成同一值的不断反复，这个在CTR中不存在这一问题。