import requests
import qrcode
import time
import json
import re

class login:
    def __init__(self, qrcode_show_timeout :int = 15, userAgent :str = 'Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0'):
        self.qrcode_getLoginUrl = 'https://passport.bilibili.com/qrcode/getLoginUrl'
        self.qrcode_getLoginInfo = 'https://passport.bilibili.com/qrcode/getLoginInfo'
        self.bilibili_home = 'https://api.bilibili.com/x/web-interface/nav'

        self.userAgent = userAgent

        self.qrcode_show_timeout = qrcode_show_timeout

    def GenerateQRCodeWithShow(self, content :str):
        qrcode.make(content).show()

    def run(self):
        results_qrcode_getLoginUrl = requests.get(self.qrcode_getLoginUrl, headers = {'User-Agent': self.userAgent}).json()

        qrcode_content = results_qrcode_getLoginUrl['data']['url']

        print(f'二维码已生成，请在{self.qrcode_show_timeout}秒内扫描二维码，内容为：{qrcode_content}')
        self.GenerateQRCodeWithShow(qrcode_content)
        time.sleep(self.qrcode_show_timeout)

        results_qrcode_getLoginInfo = requests.post(self.qrcode_getLoginInfo, headers = {'User-Agent': self.userAgent},
            data={'oauthKey': results_qrcode_getLoginUrl['data']['oauthKey']}).json()

        if results_qrcode_getLoginInfo['status'] == False:
            raise RuntimeError('错误，用户在规定时间内未扫描二维码。')

        sessdata = re.findall(r'SESSDATA=([\w\%]+)&', results_qrcode_getLoginInfo['data']['url'], re.S)[0]
        cookie = f'SESSDATA={sessdata}'
        res = requests.get(self.bilibili_home, headers={'User-Agent': self.userAgent, 'Cookie': cookie}).json()
        if res['code'] == 0:
            print(f'恭喜！你已成功登陆哔哩哔哩。你的Cookie为：{cookie}')
        else:
            print(f'错误！你未成功登陆哔哩哔哩。你的Cookie为：{cookie}')

        return res

if __name__ == '__main__':
    bili = login(qrcode_show_timeout = 5)
    bili.run()
