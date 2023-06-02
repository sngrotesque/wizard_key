# 参考资料

### AES加密算法/各模式填充情况
| 算法/模式/填充            | 16字节加密后数据长度  |  不满16字节加密后长度 |
| :---                      | :---                  | :---                  |
| AES/CBC/NoPadding         | 16                    | 不支持                |
| AES/CBC/PKCS5Padding      | 32                    | 16                    |
| AES/CBC/ISO10126Padding   | 32                    | 16                    |
| AES/ECB/NoPadding         | 16                    | 不支持                |
| AES/ECB/PKCS5Padding      | 32                    | 16                    |
| AES/ECB/ISO10126Padding   | 32                    | 16                    |
| AES/CFB/NoPadding         | 16                    | 原始数据长度          |
| AES/CFB/PKCS5Padding      | 32                    | 16                    |
| AES/CFB/ISO10126Padding   | 32                    | 16                    |
| AES/CTR/NoPadding         | 16                    | 原始数据长度          |
| AES/CTR/PKCS5Padding      | 32                    | 16                    |
| AES/CTR/ISO10126Padding   | 32                    | 16                    |
| AES/OFB/NoPadding         | 16                    | 原始数据长度          |
| AES/OFB/PKCS5Padding      | 32                    | 16                    |
| AES/OFB/ISO10126Padding   | 32                    | 16                    |
| AES/PCBC/NoPadding        | 16                    | 不支持                |
| AES/PCBC/PKCS5Padding     | 32                    | 16                    |
| AES/PCBC/ISO10126Padding  | 32                    | 16                    |

### 密码学链接

##### 设计标准
[NIST SP 800-38A](https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38a-add.pdf)



##### 基础
[加密类型：5种加密算法以及如何选择正确的算法](https://baijiahao.baidu.com/s?id=1667454526325821788)<br>
[密码学三大原则：扩散、混淆、雪崩效应](https://blog.csdn.net/qq_43699632/article/details/112009179)<br>
[TLS/SSL 协议详解 (30)](https://wonderful.blog.csdn.net/article/details/78025940)<br>
[密码学常用英文单词翻译](https://blog.csdn.net/qq_44775134/article/details/103156085)<br>
[密码学术语中英文对照](https://blog.csdn.net/qq_37397652/article/details/125265664)

##### AES
[AES加密(1): 基本AES算法](https://zhuanlan.zhihu.com/p/125633081)<br>
[AES加密(2): GF(256)域](https://zhuanlan.zhihu.com/p/125625646)<br>
[AES加密(3)：AES加密模式与填充](https://zhuanlan.zhihu.com/p/131324301)<br>

[AES算法（一）简介](https://zhuanlan.zhihu.com/p/375412170)<br>
[AES算法（二）轮函数简述](https://zhuanlan.zhihu.com/p/375682288)<br>
[AES算法（三）填充模式](https://zhuanlan.zhihu.com/p/375682512)<br>
[AES算法（四）基本工作模式](https://zhuanlan.zhihu.com/p/376077687)<br>
[AES算法（五）GCM工作模式](https://zhuanlan.zhihu.com/p/376692295)<br>
[AES算法（六）理论总结](https://zhuanlan.zhihu.com/p/396707711)

##### 公钥密码算法 / 密钥交换算法
[DH算法 | 迪菲-赫尔曼Diffie–Hellman 密钥交换](https://www.bilibili.com/video/BV1sY4y1p78s)<br>
[公钥密码的基本原理](https://blog.csdn.net/butterfly9844/article/details/77931052)<br>
[7.1公钥密码学简介](https://blog.csdn.net/qq_43699776/article/details/110240706)<br>
[公钥密码学简介](https://blog.csdn.net/daocaokafei/article/details/120633593)<br>
[详解国密SM2的加密和解密](https://blog.csdn.net/weixin_45859485/article/details/113249352)<br>
[记录RSA加解密算法使用心得](https://blog.csdn.net/yaojinjian1995/article/details/123787847)<br>
[python实现公钥加解密RSA算法(源代码层面)](https://blog.csdn.net/m0_52316372/article/details/125691807)<br>
[ECDH 算法概述（CNG 示例）](https://blog.csdn.net/earbao/article/details/119330059)<br>
[ECDH算法详解](https://blog.csdn.net/HORHEART/article/details/120443410)<br>
[四、非对称加密](https://blog.csdn.net/zq9017197/article/details/89254489)<br>
[RSA算法](https://blog.csdn.net/qq_46172668/article/details/108306289)<br>
[密码学—RSA(非对称加密)](https://blog.csdn.net/YM_1111/article/details/117899195)<br>
[RSA加密算法原理](https://blog.csdn.net/a745233700/article/details/102341542)<br>
[RSA加解密原理以及三种填充模式](http://www.kffy.cn/meiwen/293112.html)

##### 讲解
[（一）密码学](https://blog.csdn.net/weixin_42369053/article/details/127644937)<br>
[（二）DES算法 ](https://blog.csdn.net/weixin_42369053/article/details/116889434)<br>
[（三）AES算法](https://blog.csdn.net/weixin_42369053/article/details/117026909)<br>
[（四）IDEA加密算法 ](https://blog.csdn.net/weixin_42369053/article/details/117026717)<br>
[（五）分组密码工作模式 ](https://blog.csdn.net/weixin_42369053/article/details/117027803)<br>
[（六）RC4算法](https://blog.csdn.net/weixin_42369053/article/details/117028245)<br>
[（七）公钥密码-非对称加密算法 ](https://blog.csdn.net/weixin_42369053/article/details/117062670)<br>
[（八）RSA 公钥密码算法](https://blog.csdn.net/weixin_42369053/article/details/117063031)<br>
[（九）EIGamal公钥密码算法](https://blog.csdn.net/weixin_42369053/article/details/117063747)<br>
[（十）椭圆曲线加密算法ECC](https://blog.csdn.net/weixin_42369053/article/details/117064452)<br>
[（十一）国产密码算法 ](https://blog.csdn.net/weixin_42369053/article/details/117078224)<br>
[（十二）SM2椭圆曲线公钥密码算法](https://blog.csdn.net/weixin_42369053/article/details/118303907)<br>
[（十三）SM3密码杂凑算法](https://blog.csdn.net/weixin_42369053/article/details/118303945)<br>
[（十四）SM4分组密码算法](https://blog.csdn.net/weixin_42369053/article/details/118303869)<br>
[（十五）HASH算法-摘要-散列](https://blog.csdn.net/weixin_42369053/article/details/118309215)

[《现代密码学》学习笔记——第一章 引言](https://blog.csdn.net/YSL_Lsy_/article/details/125373569)<br>
[《现代密码学》学习笔记——第二章 流密码](https://blog.csdn.net/YSL_Lsy_/article/details/125434159)<br>
[《现代密码学》学习笔记——第三章 分组密码](https://blog.csdn.net/YSL_Lsy_/article/details/125481679)<br>
[《现代密码学》学习笔记——第三章 分组密码 [二] AES](https://blog.csdn.net/YSL_Lsy_/article/details/125519653)<br>
[《现代密码学》学习笔记——第三章 分组密码 [三]分组密码的运行模式](https://blog.csdn.net/YSL_Lsy_/article/details/125536088)<br>
[《现代密码学》学习笔记——第四章 公钥密码](https://blog.csdn.net/YSL_Lsy_/article/details/125546905)<br>
[《现代密码学》学习笔记——第五章 密钥分配与密钥管理](https://blog.csdn.net/YSL_Lsy_/article/details/125546924)<br>
[《现代密码学》学习笔记——第六章 消息认证和杂凑算法](https://blog.csdn.net/YSL_Lsy_/article/details/125546944)<br>
[《现代密码学》学习笔记——第七章 密钥管理[一]](https://blog.csdn.net/YSL_Lsy_/article/details/125676376)<br>
[《现代密码学》学习笔记——第七章 密钥管理[二]数字证书](https://blog.csdn.net/YSL_Lsy_/article/details/125740964)<br>
[《现代密码学》学习笔记——第八章 身份鉴别](https://blog.csdn.net/YSL_Lsy_/article/details/125747409)<br>
[《现代密码学》学习笔记——关于时间复杂度的补充内容](https://blog.csdn.net/YSL_Lsy_/article/details/125504226)

[精读《图解密码技术》--第一章 环游密码世界](https://blog.csdn.net/YSL_Lsy_/article/details/124944113)<br>
[精读《图解密码技术》--第二章 历史上的密码](https://blog.csdn.net/YSL_Lsy_/article/details/124969068)<br>
[精读《图解密码技术》--第三章 对称密码（共享密钥密码）](https://blog.csdn.net/YSL_Lsy_/article/details/125004244)<br>
[精读《图解密码技术》--第四章 分组密码的模式](https://blog.csdn.net/YSL_Lsy_/article/details/125163366)<br>
[精读《图解密码技术》--第五章 公钥密码](https://blog.csdn.net/YSL_Lsy_/article/details/125185925)<br>
[精读《图解密码技术》--第六章 混合密码系统](https://blog.csdn.net/YSL_Lsy_/article/details/125209479)<br>
[精读《图解密码技术》--第七章 单项散列函数](https://blog.csdn.net/YSL_Lsy_/article/details/125216357)<br>
[精读《图解密码技术》——第八章 消息认证码](https://blog.csdn.net/YSL_Lsy_/article/details/125258031)<br>
[精读《图解密码技术》——第九章 数字签名](https://blog.csdn.net/YSL_Lsy_/article/details/125263957)<br>
[精读《图解密码技术》——第十章 证书](https://blog.csdn.net/YSL_Lsy_/article/details/125278581)<br>
[精读《图解密码技术》——第十一章 密钥](https://blog.csdn.net/YSL_Lsy_/article/details/125299044)<br>
[精读《图解密码技术》——第十三章 PGP](https://blog.csdn.net/YSL_Lsy_/article/details/125324076)<br>
[精读《图解密码技术》——第十四章 SSL/TLS](https://blog.csdn.net/YSL_Lsy_/article/details/125338670)


好的，SN，我可以给你一些关于这些加密算法的实现细节的链接，你可以点击查看更多内容。123

AES算法的实现细节可以参考这个链接：https://blog.csdn.net/qq_28205153/article/details/55798628

SM4算法的实现细节可以参考这个链接：https://blog.csdn.net/u013248535/article/details/56011024

RSA算法的实现细节可以参考这个链接：https://blog.csdn.net/weixin_43734095/article/details/104725548

SM2算法的实现细节可以参考这个链接：https://www.cnblogs.com/pythonywy/p/13999474.html

希望这些信息对你有帮助。😊
