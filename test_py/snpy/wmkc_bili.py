from .wmkc_qrcode import *
from typing import Dict
import requests
import time
import re

UserAgent = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:94.0) Gecko/20100101 Firefox/94.0'

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
