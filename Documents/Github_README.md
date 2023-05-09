# 🤗 调试 🤗❤️
<img src="https://www.murphysec.com/platform3/v3/badge/1611431819275173888.svg" align="right">

### 如果你使用我的代码时遇到问题，请参考StackOverflow的提问姿势。

> 具体可以参考[Angelina-Bot](https://www.angelina-bot.top/About.html)的智力对照表

| 智力得分	| 提问方式 |
| :--- | :--- |
| 100分	| 我在xxx遇到了一个问题，在翻阅文档和视频后，这里是报错截图，我参考了百度或谷歌中xxx文章进行解决，但仍不能解决 |
| 90分	| 我遇到了xxx问题，我翻阅了文档和视频，没有找到解决方案，完整的报错截图如下 |
| 60分	| 这里是完整的报错截图，请问应该怎么解决 |
| 10分	| 这咋回事啊（手机拍屏幕）|
| 0分	| 这怎么办啊（不完整的报错截图） |
| sb	| 大神求带、这怎么不能用了、我打不开了、你们这个正常吗、文件在哪啊、怎么学啊、我是小白能帮我吗 |

# 关于 🍺
<a href="https://stackoverflow.com/users/21376217/s-n"><img src="https://stackoverflow.com/users/flair/21376217.png" width="208" height="58" alt="profile for S-N at Stack Overflow, Q&amp;A for professional and enthusiast programmers" title="profile for S-N at Stack Overflow, Q&amp;A for professional and enthusiast programmers" align="right"></a>

你好~我是SN-Grotesque，欢迎来我的主页。❤<br>
我是一个喜欢代码喜欢绘画喜欢音乐的人，同时也是一个视频博主。<br>
我喜欢研究计算机的深层技术与实现算法，渴望使用最短代码完成最高水准。<br>

我的公开仓库基本都是一些普通的工具，目前并不会将私有仓库开放。<br>
我的加密算法与部分其他的开发资源均在私有仓库。

以下为我的个人主页相关网站传送门

<table>
    <tr>
        <td>Pixiv</td>      <td>Youtube</td>     <td>哔哩哔哩</td>    <td>Soundcloud</td>    <td>PixivFanbox</td>
        <td>Twitter</td>    <td>网易云音乐</td>   <td>知乎</td>       <td>爱发电</td>         <td>电子邮箱</td>
    </tr>
    <tr>
        <td><a href="https://www.pixiv.net/users/38279179"><img src="https://www.pixiv.net/favicon.ico" width="15px" height="15px"></a></td>
        <td><a href="https://www.youtube.com/channel/UCITRiFd37VZS8y4vjW2pfYQ/featured">
            <img src="https://www.youtube.com/favicon.ico" width="15px" height="15px"></a></td>
        <td><a href="https://space.bilibili.com/27958784"><img src="https://www.bilibili.com/favicon.ico" width="15px" height="15px"></a></td>
        <td><a href="https://soundcloud.com/sngrotesque"><img src="https://soundcloud.com/favicon.ico" width="15px" height="15px"></a></td>
        <td><a href="https://sng.fanbox.cc/"><img src="https://sng.fanbox.cc/favicon.ico" width="15px" height="15px"></a></td>
        <td><a href="https://twitter.com/SNGOfficial4"><img src="https://twitter.com/favicon.ico" width="15px" height="15px"></a></td>
        <td><a href="https://music.163.com/#/user/home?id=1686139386">
            <img src="http://s1.music.126.net/style/favicon.ico" width="15px" height="15px"></a></td>
        <td><a href="https://www.zhihu.com/people/kianakaslana-16"><img src="https://www.zhihu.com/favicon.ico" width="15px" height="15px"></a></td>
        <td><a href="https://afdian.net/@sngrotesque"><img src="https://afdian.net/favicon.ico" width="15px" height="15px"></a></td>
        <td><a href="mailto:kq-cos@outlook.com">E-Mail</a></td>
    </tr>
</table>

```c
SN_STATIC_FUNC(snError) snBase64Encode(snObject *dst, snObject *src)
{
    if(!dst->buf || !src->buf || !src->size) {
        printf("dst or src or src->size is null.\n");
        return snErr_ErrNullData;
    }
    snSize dst_i, src_i;

    for (dst_i = src_i = 0; dst_i < dst->size - 2; src_i += 3, dst_i += 4) {
        dst->buf[dst_i]   = _B64ET[src->buf[src_i] >> 2];
        dst->buf[dst_i+1] = _B64ET[(src->buf[src_i] & 0x3)   << 4 | (src->buf[src_i+1] >> 4)];
        dst->buf[dst_i+2] = _B64ET[(src->buf[src_i+1] & 0xf) << 2 | (src->buf[src_i+2] >> 6)];
        dst->buf[dst_i+3] = _B64ET[src->buf[src_i+2] & 0x3f];
    }

    switch(src->size % 3) {
        case 1:
            dst->buf[dst_i - 2] = BASE64_PAD;
            dst->buf[dst_i - 1] = BASE64_PAD;
            break;
        case 2:
            dst->buf[dst_i - 1] = BASE64_PAD;
            break;
    }

    return snErr_OK;
}
```

# 技能 🍻
<img src="https://github-readme-stats.vercel.app/api/top-langs?username=sngrotesque&layout=compact" align="right">

主要使用的语言有`C/C++/Python`。<br>
偶尔使用或正在学习的语言有`Java/C#/Ruby/Rust/Perl`。<br>
正在学习游戏开发，建模与图形学。<br>
目前完成了对称加密算法，图像生成工具，与网络爬虫的开发。

未来用作游戏开发和底层程序开发主要使用的框架与引擎为：`Qt`，`Unity`，`Unreal Engine`，`OpenGL`，`OpenSSL`。

# 未来目标

- 网络安全工程师 - Network Security Engineer
- 画手 - Painter
- 全栈工程师 - Full Stack Developer
- 游戏开发者 - Game developers

# 爱好 🥂
<img src="https://github-readme-stats.vercel.app/api?username=sngrotesque&show_icons=true&count_private=true&theme=cobalt&show_icons=true" align="right">

1. Cosplay (角色扮演)
2. 喝酒喝饮料
3. 看《海绵宝宝》
4. 做爱

如果你觉得我的项目不错，那么你可以赞助我🍗/❤/💕，非常感谢！<br>
| [爱发电 - Afdian](https://afdian.net/@sngrotesque) | [PixivFanbox](https://sng.fanbox.cc/) |
| --------------                                    | -----------                           |
> Thank you very much.
