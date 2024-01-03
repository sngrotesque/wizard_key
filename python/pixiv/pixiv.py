from typing import Union, List, Callable
from zipfile import ZipFile
import threading
import requests
import json
import sys
import cv2
import re
import os

STATUS_DONE = 1
STATUS_EXISTS = -1
STATUS_FAILED = 0

def fread(path :str, mode :str = 'r', encoding = None) -> Union[str, bytes]:
    with open(path, mode, encoding = encoding) as f:
        return f.read()

def fwrite(path :str, content :Union[str, bytes], mode :str = 'w', encoding = None) -> bool:
    with open(path, mode, encoding = encoding) as f:
        f.write(content)
    return True

def fwrite_json(path :str, json_data :dict):
    fwrite(path, json.dumps(json_data, ensure_ascii = False))

def get_user_agent_list(path :str = 'useragent.txt') -> List[str]:
    return fread(path).split()

def kill_self():
    if sys.platform == 'win32':
        return fwrite('kill_pixiv_py.bat', f'@echo off\ntaskkill /f /pid {os.getpid()}')
    return fwrite('kill_pixiv_py.sh', f'#!/bin/bash\nkill -9 {os.getpid()}')

class pixiv:
    '''
    这里需要着重声明一个事情，如果你在调用getTotalArtistList方法后发现得到的数量少于Pixiv官网的数量。
    这不是我代码的问题，这是Pixiv的问题。
    不信的话你可以在getArtistList方法中检测每一页的作者数量然后和Pixiv官网的那一页的数量作对比。
    '''
    def __init__(self, myID :Union[str, int],
                cookies_path :str = None,
                cookies_data :str = None,
                proxies :str = 'http://localhost:1080',
                save_path :str = '.',
                maxNumberThreads :int = 8):
        self.maxNumberThreads = maxNumberThreads
        self.myself_id = myID
        self.save_path = save_path

        if cookies_path:
            self.cookies = fread(cookies_path)
        elif cookies_data:
            self.cookies = cookies_data
        else:
            raise RuntimeError('缺少Cookie，无法进行爬取。')

        self.proxies = {'http': proxies, 'https': proxies} if proxies else None
        self.headers = {
            'Cookie': self.cookies,
            'Accapt-Language': 'zh-CN, zh;q=0.9, en;q=0.8, en-GB;q=0.7, en-US;q=0.6',
            'Referer': 'https://www.pixiv.net/',
            'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/116.0'
        }

    # 开启指定数量的线程并执行
    def threads(self, func :Callable, url_list :List[str]):
        th_list = [threading.Thread(target = func, args = (thId, url_list)) for thId in range(self.maxNumberThreads)]

        for th in th_list:
            th.start()

        for th in th_list:
            th.join()

    # 封装HTTP请求
    def http_get(self, url :str):
        return requests.get(url, headers = self.headers, proxies = self.proxies)

    # 根据下载链接来创建一个文件名
    def createFileName(self, url :str):
        return re.findall(r'\w+://[a-zA-Z0-9.\-\_]+/[a-zA-Z\-\_]+/img/([0-9a-zA-Z./\_]+)',
            url, re.S | re.I)[0].replace('/', '_')

    # 指定一个Jpg图像文件路径的列表，将它们依次转为视频的每一帧
    def jpgToMp4(self, inPath :List[str], outPath :str, fps :int = 15):
        img_array = []

        for filename in inPath:
            img = cv2.imread(filename)
            height, width, layers = img.shape
            img_array.append(img)

        out = cv2.VideoWriter(outPath, cv2.VideoWriter_fourcc(*'DIVX'), fps, (width, height))

        for i in img_array:
            out.write(i)

        out.release()

    # 将Zip压缩包里面的图像转为视频
    def zipToMp4(self, link :str, fileSavePath :str, content :Union[ZipFile, bytes], folder :str):
        fwrite(fileSavePath, content, mode = 'wb')

        with ZipFile(fileSavePath, 'r') as ctx:
            zip_filename_list = ctx.namelist()
            jpgToMp4TempSavePath = f'{fileSavePath}_Temp'

            if not os.path.exists(jpgToMp4TempSavePath):
                os.makedirs(jpgToMp4TempSavePath)

            ctx.extractall(jpgToMp4TempSavePath)

        jpgPath = [os.path.join(jpgToMp4TempSavePath, fn) for fn in zip_filename_list]
        gifFileName = self.createFileName(link).replace('zip', 'mp4')

        self.jpgToMp4(jpgPath, os.path.join(folder, gifFileName))

        for fn in jpgPath:
            os.remove(fn)

        os.remove(fileSavePath)
        os.removedirs(jpgToMp4TempSavePath)

    # 获取指定页码中所有作者ID
    def getArtistList(self, page :int, offset :int) -> List[str]:
        res = self.http_get(f'https://www.pixiv.net/ajax/user/{self.myself_id}/following'
            f'?offset={page * offset}&limit={offset}&rest=show').json()

        # 如果获取完毕就返回一个False
        if not res['body']['users']:
            return False
        # 否则返回当前页获取的ID列表
        return [index['userId'] for index in res['body']['users']]

    # 获取自己关注的所有作者的ID
    def getTotalArtistList(self, offset :int = 24):
        results = []
        for page in range(0, int(offset * 1e6)):
            result = self.getArtistList(page, offset)
            if not result:
                break
            results += result
        return results

    # 多线程获取指定作者的所有作品中的所有图像的链接
    # 这个方法如果改为单线程，那么速度堪忧并且没法兼容多线程下载图像
    def getIllustsImagesLink(self, artistID :Union[str, int]):
        artworks = [*self.http_get(f'https://www.pixiv.net/ajax/user/{artistID}/profile/all'
            f'?lang=zh').json()['body']['illusts'].keys()]

        self.links = []
        def get_images(thID :int, artworks :List[str]):
            for pid in range(thID, len(artworks), self.maxNumberThreads):
                print(f'Thread[{thID:02x}] obtains the image link in PID[{artworks[pid]}].')
                dynamicImageUrl = f'https://www.pixiv.net/ajax/illust/{artworks[pid]}/ugoira_meta?lang=zh'
                staticImageUrl = f'https://www.pixiv.net/ajax/illust/{artworks[pid]}/pages?lang=zh'

                dynamicImage_result = self.http_get(dynamicImageUrl).json()
                statucImage_result = self.http_get(staticImageUrl).json()

                if not dynamicImage_result['error']:
                    self.links.append(dynamicImage_result['body']['originalSrc'])
                else:
                    for index in statucImage_result['body']:
                        self.links.append(index['urls']['original'])

        self.threads(get_images, artworks)

        return self.links

    # 下载单个作品（提供重连机制）
    def download(self, url :str, zipToMp4 :bool = False, ReSpecifyPath :str = None, retry_count :int = 5):
        # 如果用户指定了新的保存路径就使用新的路径以覆盖类中的save_path
        if ReSpecifyPath:
            self.save_path = ReSpecifyPath

        if not os.path.exists(self.save_path):
            os.makedirs(self.save_path)

        fileSavePath = os.path.join(self.save_path, self.createFileName(url))

        if os.path.exists(fileSavePath) or os.path.exists(fileSavePath.replace('zip', 'mp4')):
            return STATUS_EXISTS

        while True:
            try:
                response = self.http_get(url)
                break
            except:
                if not retry_count:
                    return STATUS_FAILED
                retry_count -= 1

        if response.headers['Content-Type'] == 'application/zip':
            if zipToMp4:
                self.zipToMp4(url, fileSavePath, response.content, self.save_path)
            else:
                fwrite(fileSavePath, response.content, mode = 'wb')
        else:
            fwrite(fileSavePath, response.content, mode = 'wb')

        return STATUS_DONE

    # 多线程下载指定作者的所有作品
    def multiThreadedDownload(self, artistID :Union[str, int], ReSpecifyPath :str = None):
        def _download(thID :int, links :List[str]):
            for index in range(thID, len(links), self.maxNumberThreads):
                fn = self.createFileName(links[index])
                print(f'Thread[{thID:02x}] download \'{fn}\'')
                status = self.download(links[index], zipToMp4 = True, ReSpecifyPath = ReSpecifyPath)
                if status == STATUS_EXISTS:
                    print(f'Thread[{thID:02x}] download \'{fn}\', Exists.')
                elif status == STATUS_FAILED:
                    print(f'Thread[{thID:02x}] download \'{fn}\', Failed.')
        self.threads(_download, self.getIllustsImagesLink(artistID))

kill_self()

myself_id = 38279179
artist_id = 58131017
cookies_data = ''

artist_id_list = [
    # '81048301',
    '57822259'
]

px = pixiv(myself_id, cookies_data = cookies_data, save_path = 'pixiv_save', proxies = 'http://localhost:8081/')
for artistId in artist_id_list:
    px.multiThreadedDownload(artistId, f'pid_{artistId}')

