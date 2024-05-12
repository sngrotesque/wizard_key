from typing import Union, List
from lxml import etree
import requests
import os
import re

XPATH_TITLE   = '/html/head/title/text()'            # 匹配网页标题
XPATH_SHOW    = '/html/body/div/div/div/a/@href'     # 匹配单页所有图片的查看链接
XPATH_PICTURE = '/html/body/div/div/a/img/@src'      # 通过查看链接获取页面中的图片
XPATH_PAGE    = '/html/body/div/table/tr/td/a/@href' # 匹配总页数

def fwrite(path :str, content :Union[str, bytes], mode :str = 'w', encoding = None) -> bool:
    with open(path, mode, encoding = encoding) as f:
        f.write(content)
    return True

def xpath_parse(data :str, xpath_content :str):
    return etree.HTML(data).xpath(xpath_content)

def createFileName(url :str):
    return re.findall(r'.*/([\w\d\_.]+)$', url, re.I | re.S)[0]

class ehentai:
    def __init__(self, proxies :str = 'http://localhost:1080/', save_path :str = '.', maxNumberThreads :int = 8):
        self.proxies = {'http': proxies, 'https': proxies} if proxies else None
        self.headers = {
            'Accapt-Language': 'zh-CN, zh;q=0.9, en;q=0.8, en-GB;q=0.7, en-US;q=0.6',
            'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:97.0) Gecko/20100101 Firefox/97.0'
        }
        self.save_path = save_path
        self.maxNumberThreads = maxNumberThreads

    def http_get(self, url :str):
        return requests.get(url, headers = self.headers, proxies = self.proxies)

    def getAllPages(self, url :str):
        html_content = self.http_get(url).text
        pageNumber = sorted(list(set(xpath_parse(html_content, XPATH_PAGE))))
        print(pageNumber)
        return pageNumber

    def getAllArtworkLists(self, url_list :List[str]):
        artwork_list = []
        for page in url_list:
            html_content :List[str] = self.http_get(page).text
            for artwork in xpath_parse(html_content, XPATH_SHOW):
                artwork_list.append(artwork)
        return artwork_list

    def download(self, url_list :List[str]):
        print(f'执行download方法')

        if not os.path.exists(self.save_path):
            os.makedirs(self.save_path)

        for artwork in url_list:
            html_content = self.http_get(artwork).text
            link = xpath_parse(html_content, XPATH_PICTURE)[0]

            fn = os.path.join(self.save_path, createFileName(link))
            if os.path.exists(fn):
                print(f'\'{fn}\' exists.')
                continue

            fwrite(fn, self.http_get(link).content, mode = 'wb')

ctx = ehentai(save_path = 'e_save')
res = ctx.getAllPages('https://e-hentai.org/g/2786901/9aa74c9e05/')
res = ctx.getAllArtworkLists(res)
res = ctx.download(res)


