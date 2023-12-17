from typing import Union, List
from zipfile import ZipFile
import requests
import random
import json
import cv2
import re
import os

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

class pixiv:
    '''
    这里需要着重声明一个事情，如果你在调用getTotalArtistList方法后发现得到的数量少于Pixiv官网的数量。
    这不是我代码的问题，这是Pixiv的问题。
    不信的话你可以在getArtistList方法中检测每一页的作者数量然后和Pixiv官网的那一页的数量作对比。
    '''
    def __init__(self, myID :Union[str, int],
                cookies_path :str = 'cookie.txt',
                proxies :str = 'http://localhost:1080',
                maxNumberThreads :int = 8
                ):
        self.maxNumberThreads = maxNumberThreads

        self.cookies = fread(cookies_path)
        self.proxies = {'http': proxies, 'https': proxies}

        self.myself_id = myID

        self.headers = {
            'Cookie': self.cookies,
            'Accapt-Language': 'zh-CN, zh;q=0.9, en;q=0.8, en-GB;q=0.7, en-US;q=0.6',
            'Referer': 'https://www.pixiv.net/',
            'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/116.0'
        }

    def http_get(self, url :str):
        return requests.get(url, headers = self.headers, proxies = self.proxies)

    # 根据下载链接来创建一个文件名
    def createFileName(self, url :str):
        return re.findall(r'\w+://[a-zA-Z0-9.\-\_]+/[a-zA-Z\-\_]+/img/([0-9a-zA-Z./\_]+)',
            url, re.S | re.I)[0].replace('/', '_')

    # 指定一个Jpg图像的列表，将它们转为视频的每一帧
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

            print(f'82 line: {jpgToMp4TempSavePath = }')

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

        if not res['body']['users']:
            return False

        return [index['userId'] for index in res['body']['users']]

    # 获取自己关注的所有作者的ID
    def getTotalArtistList(self, offset :int = 24):
        results = []
        for page in range(0, int(offset * 1e6)):
            print(f'page number: {page + 1}')
            result = self.getArtistList(page, offset)
            if not result:
                print('done.')
                break
            results += result
        return results

    # 获取指定作者主页中所有的作品ID
    def getArtworksIllusts(self, artistID :Union[str, int]):
        return [*self.http_get(f'https://www.pixiv.net/ajax/user/{artistID}/profile/all'
            f'?lang=zh').json()['body']['illusts'].keys()]

    # 获取指定作品ID中的所有图像的链接
    def getIllustsImagesLink(self, artworkID :Union[str, int]):
        dynamicImageUrl = f'https://www.pixiv.net/ajax/illust/{artworkID}/ugoira_meta?lang=zh'
        staticImageUrl = f'https://www.pixiv.net/ajax/illust/{artworkID}/pages?lang=zh'

        dynamicImage_result = self.http_get(dynamicImageUrl).json()
        statucImage_result = self.http_get(staticImageUrl).json()

        links = []
        if not dynamicImage_result['error']:
            links.append(dynamicImage_result['body']['originalSrc'])
        else:
            for index in statucImage_result['body']:
                links.append(index['urls']['original'])

        return links

    # 下载单个作品
    def download(self, url :str, savePath :str = '.', zipToMp4 :bool = False):
        # 如果用户指定的保存目录不存在的话就递归创建
        if not os.path.exists(savePath):
            os.makedirs(savePath)

        fileSavePath = os.path.join(savePath, self.createFileName(url))

        # 如果路径已存在，则不重复保存
        if os.path.exists(fileSavePath) or os.path.exists(fileSavePath.replace('zip', 'mp4')):
            return False

        # 获取内容
        response = self.http_get(url)

        if response.headers['Content-Type'] == 'application/zip':
            if zipToMp4:
                print(f'将zip转为mp4')
                self.zipToMp4(url, fileSavePath, response.content, savePath)
            else:
                fwrite(fileSavePath, response.content, mode = 'wb')
        else:
            fwrite(fileSavePath, response.content, mode = 'wb')

        return True

myself_id = 38279179
artist_id = 58131017
artwork_id = 114304146
save_path = 'Y:/pixiv_save/artwork_id_70250134/'
cookie_path = './pixiv_cookie.txt'
links = [
    'https://i.pximg.net/img-zip-ugoira/img/2018/08/17/22/06/00/70250134_ugoira1920x1080.zip'
]

px = pixiv(38279179, cookies_path = cookie_path)
for x in links:
    res = px.download(x, save_path, zipToMp4 = True)
    print(res)
