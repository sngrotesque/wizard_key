from .wmkc_file import wmkcFile_fread, wmkcFile_fwrite
from .wmkc_qrcode import *
from typing import Dict
from lxml import etree
import requests
import time
import re

UserAgent = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:94.0) Gecko/20100101 Firefox/94.0'
splitLine = [
    '6aa3cd9fae5e68fe6c4adfb0ad632163d5054e4cf1c1c4feb5e8c8c66f7fc1a3',
    '8f58faf656e41f9c56e47784e8553458791c4f6aa41123970011f35613eea9e2',
    '59e48d6a98060684125675f95693ebaaf269b83557f375b557c34a7aae46e0b3',
    'bc9744b7d60fad53c710972d90c474af56d5237f113158b6b1ebf357c5b116a8',
    'e6d75bd7b9d859f75111c0c648d10b2918c7c94049bcc6be37166d3a1f94bcfc',
    'f4078e598cbb4bfdef15def2354b6f2b2bea8291a0289ffec94ac805460192d4'
]

def xpath(html_text :str, xpath_parser :str):
    return etree.HTML(html_text).xpath(xpath_parser)

class wmkcBili:
    def __init__(self, userAgent :str = UserAgent):
        self.headers = {
            'Cookie': None,
            'Referer': 'https://www.bilibili.com/',
            'User-Agent': userAgent
        }

    def login(self, QRcodeSuspensionTime :float = 7):
        def qrcode_show(content: str):
            qr = wmkcQrcode(box_size = 4, error_correction = QRCODE_ERROR_L)
            qr.init()
            qr.add(content)
            qr.make_text()
            qr.show()
            time.sleep(QRcodeSuspensionTime)

        oauthKey_res = requests.get('https://passport.bilibili.com/qrcode/getLoginUrl',
                            headers = {'User-Agent': UserAgent}).json()

        qrcode_show(oauthKey_res['data']['url'])

        login_res = requests.post('https://passport.bilibili.com/qrcode/getLoginInfo',
                            headers = {'User-Agent': UserAgent},
                            data = {'oauthKey': oauthKey_res['data']['oauthKey']}).json()

        try:
            SESSDATA = re.findall(r'SESSDATA=([\w\d\%]{34,36})&', login_res['data']['url'], re.I | re.S)[0]
        except TypeError:
            print(f'用户没有扫描二维码，此为服务器前段返回的内容：{login_res}')
            return -1

        self.headers['Cookie'] = f'SESSDATA={SESSDATA}'
        return self.headers

    def downloadArticleImages(self, cvid :int = None):
        ## B站似乎把专栏相关的API改变了，没法使用以前的方法来获取了
        ## 目前等待我再次完善这个爬虫
        url = f'https://www.bilibili.com/read/cv{cvid}/'
        content = requests.get(url, headers = self.headers).content

        wmkcFile_fwrite('text.html', content)

