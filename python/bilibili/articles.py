from typing import Union, List, Callable
from lxml import etree
import concurrent.futures # 后续使用此库来使用线程池进行爬取
import requests
import hashlib
import typing
import time
import os
import re

ARTICLE_PAGE_MIN = 1
ARTICLE_PAGE_MAX = 16777216

# 注意，此列表为哔哩哔哩官方提供的分割线图像的SHA1摘要。
# 无法用于过滤UP主本人自定义的分割线风格图像，如需过滤，请将图像的SHA1摘要添加进此列表
bilibili_article_splitLine_sha256_hexdigest = [
    '0117cbba35e51b0bce5f8c2f6a838e8a087e8ee7',
    '4aa545dccf7de8d4a93c2b2b8e3265ac0a26d216',
    '71bf2cd56882a2e97f8b3477c9256f8b09f361d3',
    'db75225feabec8d8b64ee7d3c7165cd639554cbc',
    '4adb9255ada5b97061e610b682b8636764fe50ed',
    '02db465212d3c374a43c60fa2625cc1caeaab796']

def getSHA1_Hexdigest(content :Union[bytes, bytearray]):
    if not isinstance(content, (bytes, bytearray)):
        raise SN_BLBL_Exception(f'请不要传入非bytes与bytearray类型。')
    return hashlib.sha256(content).hexdigest()

class SN_BLBL_Exception(Exception): ...

class articles:
    def __init__(self, uid :typing.Union[str, int], imageSavePath: str = '.', overwriteFile :bool = False):
        self.up_uid = uid
        self.imageSavePath = imageSavePath
        self.overwriteFile = overwriteFile
        
        self.userAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0'
        self.httpHeaders = {
            'User-Agent': self.userAgent
        }

    def getArticleCVList(self, page_number :int = 1) -> typing.List[str]:
        if page_number < ARTICLE_PAGE_MIN:
            raise IndexError('最小的页码数应该为1。')
        url = f'https://api.bilibili.com/x/space/article?mid={self.up_uid}&pn={page_number}&ps=12&sort=publish_time'
        try:
            return [f'https://www.bilibili.com/read/cv{index["id"]}'
                for index in requests.get(url, headers = self.httpHeaders).json()['data']['articles']]
        except KeyError:
            return False

    def getArticleCVPicturesLink(self, cv_url :str):
        res = requests.get(cv_url, headers = self.httpHeaders).text
        tmp_results = etree.HTML(res).xpath('/html/body/div/div/div/div/div/div/figure/img/@data-src')
        return [f'https:{x}' for x in tmp_results]

    def download(self, url :str):
        res = requests.get(url, headers = self.httpHeaders)
        if res.status_code != 200:
            raise SN_BLBL_Exception(f'服务器返回了意料之外的响应码：{res.status_code}')

        if getSHA1_Hexdigest(res.content) in bilibili_article_splitLine_sha256_hexdigest:
            return False

        if not os.path.exists(self.imageSavePath):
            os.mkdir(self.imageSavePath)

        imageFileName = re.findall(r'^\w+://[a-zA-Z0-9.]+/[\w\/]+/(.*)$', url)[0]
        path = f'{self.imageSavePath}/{imageFileName}'

        if os.path.exists(path) and (not self.overwriteFile):
            raise SN_BLBL_Exception(f'\'{path}\'已存在，由于设置为未覆盖文件，放弃保存图像。')

        with open(path, 'wb') as f:
            f.write(res.content)

        return True

    def multiThreadDownload(self, maxNumberThreads :int = 8):
        def _get_article_cv(start :int, stop :int, step :int):
            result = []
            for page_number in range(start, stop, step):
                cvLinkList = self.getArticleCVList(page_number + 1)
                if not cvLinkList:
                    break
                result += cvLinkList
            return result

        def _get_article_image():
            pass

        def _download(url :List[str], start :int, stop :int, step :int, download_yes :bool = False):
            pass

        def multiThreadCall(func :Callable, content_len :int):
            with concurrent.futures.ThreadPoolExecutor(max_workers = maxNumberThreads) as executor:
                futures = [executor.submit(func, threadId, content_len, maxNumberThreads) for threadId in range(maxNumberThreads)]
                return [x.result() for x in concurrent.futures.as_completed(futures)]

        # 这里会返回每个线程调用_get_article_cv函数的结果，并组成为二位列表，所以需要处理一下解包的问题
        res = multiThreadCall(_get_article_cv, ARTICLE_PAGE_MAX)
        for x in res:
            print(x)

def demo(uid :int, imageSavePath :str):
    blbl = articles(uid, imageSavePath)
    for page_number in range(ARTICLE_PAGE_MIN, ARTICLE_PAGE_MAX):
        print(f'获取第{page_number}页的专栏....')
        cvLinkList = blbl.getArticleCVList(page_number)

        if not cvLinkList:
            print(f'此UP主的专栏已获取完毕。')
            break

        for cvLink in cvLinkList:
            print(f'获取第{page_number}页的专栏\'{cvLink}\'中的图像链接...')

            imageLinkList = blbl.getArticleCVPicturesLink(cvLink)
            for imageLink in imageLinkList:
                try:
                    if not blbl.download(imageLink):
                        print(f'跳过保存哔哩哔哩分割线，链接：{imageLink}')
                    else:
                        print(f'保存\'{imageLink}\'成功')
                except SN_BLBL_Exception as e:
                    print(e)

if __name__ == '__main__':
    # demo(1177593795, '哔哩哔哩-测试')
    blbl = articles(512675827)
    
    start = time.time()
    blbl.multiThreadDownload()
    stop = time.time()
    print(f'用时：{stop-start:.2f}')