UserAgent = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:94.0) Gecko/20100101 Firefox/94.0'

def qrcode_show(content: str, _time_sleep :float):
    img = qrcode.make(content)
    img.show()
    time.sleep(_time_sleep)

def bili_login():
    url_getLoginUrl = 'https://passport.bilibili.com/qrcode/getLoginUrl'
    url_getLoginInfo = 'https://passport.bilibili.com/qrcode/getLoginInfo'
    headers_getLoginUrl = {
        'User-Agent': UserAgent
    }

    res = requests.get(url_getLoginUrl, headers = headers_getLoginUrl).json()
    qrcode_show(res['data']['url'], 6)

    headers_getLoginInfo = {'User-Agent': UserAgent}
    data_getLoginInfo = {'oauthKey': res['data']['oauthKey']}

    res = requests.post(url_getLoginInfo, headers = headers_getLoginInfo, data = data_getLoginInfo).json()

    return res