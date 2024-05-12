# Warning ####################################################
# 请注意，这个代码只能获取哔哩哔哩专栏中的图片，而不能获取动态中的图片！###
# End ########################################################
from typing import Union, List, Callable
from lxml import etree
import concurrent.futures # 后续使用此库来使用线程池进行爬取
import requests
import os
import re

bili_splitLine_hexdigest = [
    '0117cbba35e51b0bce5f8c2f6a838e8a087e8ee7',
    '4aa545dccf7de8d4a93c2b2b8e3265ac0a26d216',
    '71bf2cd56882a2e97f8b3477c9256f8b09f361d3',
    'db75225feabec8d8b64ee7d3c7165cd639554cbc',
    '4adb9255ada5b97061e610b682b8636764fe50ed',
    '02db465212d3c374a43c60fa2625cc1caeaab796'
]

class articles:
    def __init__(self, uid :Union[str, int], savePath :str, overwrite :bool = False):
        self.uid = uid
        self.path = savePath
        self.overwrite = overwrite

        self.headers = {
            'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0'
        }

    def get_CV_list(self, page :int = 1):
        if page < 1:
            raise ValueError(f'页码数必须大于0')

        url = f'https://api.bilibili.com/x/space/article?mid={self.uid}&pn={page}&ps=12&sort=publish_time'

        try:
            return [f'https://www.bilibili.com/read/cv{index["id"]}'
                for index in requests.get(url, headers = self.headers).json()['data']['articles']]
        except KeyError:
            raise

    def get_CV_image_list(self, cv_url :str):
        local_hexdigest = bili_splitLine_hexdigest
        responsive = requests.get(cv_url, headers = self.headers).text
        results = etree.HTML(responsive).xpath('/html/body/div/div/div/div/div/div/figure/img/@data-src')
        return [f'https:{x}' for x in results if x[-44:-4] not in local_hexdigest]

    def download(self, url :str):
        res = requests.get(url, headers = self.headers)

        if res.status_code != 200:
            raise RuntimeError(f'服务器返回了意料之外的响应码：{res.status_code}')

        if not os.path.exists(self.path):
            os.mkdir(self.path)

        imageFileName = re.findall(r'^\w+://[a-zA-Z0-9.]+/[\w\/]+/(.*)$', url)[0]
        path = os.path.join(self.path, imageFileName)

        if os.path.exists(path):
            return False

        with open(path, 'wb') as f:
            f.write(res.content)

        return True

ctx = articles(341524801, 'download_test')
# for page in range(2**24):
#     cv_list = ctx.get_CV_list(page+1)
#     for cv in cv_list:
#         cv_img_list = ctx.get_CV_image_list(cv)
#         for url in cv_img_list:
#             print(f'{url}: {ctx.download(url)}')
