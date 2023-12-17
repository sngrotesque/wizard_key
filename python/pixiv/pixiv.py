from typing import Union, List
from lxml import etree
import requests
import random
import json
import os

# Utils.py Begin ##########################################################
from requests import get as rget
from json     import dumps
from zipfile  import ZipFile
from os.path  import exists
from os       import mkdir, remove, rmdir
from typing   import Union, List
import ctypes, cv2, re, threading

RED, GOLD, CYAN, RESET = "\x1b[91m", "\x1b[93m", "\x1b[96m", "\x1b[0m"

UserAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:97.0) Gecko/20100101 Firefox/97.0'
Referer   = 'https://www.pixiv.net/'

# 预览Pixiv中的指定数据(图片，压缩包等)
def PixivPreview(url :str, DirectoryName :str, OpenTheFile :bool = True):
    ProxyInfo = {"http": "http://127.0.0.1:1080", "https": "http://127.0.0.1:1080"}
    
    print(f'>>>> 检查{DirectoryName}目录...')
    if not exists(DirectoryName):
        print(f'>>>> 不存在{DirectoryName}目录，创建.')
        mkdir(DirectoryName)
    
    print(f'>>>> 为文件创建名称.')
    fileName = re.findall( # 为文件命名
        r'\w+://[a-zA-Z0-9.\-\_]+/[a-zA-Z\-\_]+/img/'
        r'([0-9a-zA-Z./\_]+)', url, re.S | re.I)[0]
    fileName = fileName.replace('/', '_')
    fileName = f'000_{fileName}'
    fileName = f'{DirectoryName}/{fileName}'
    
    print(f'>>>> 获取文件数据.')
    imgData = rget(url, headers = {'user-agent': UserAgent, 'Referer': Referer},
        proxies = ProxyInfo).content
    print(f'>>>> 保存至文件: {fileName}')
    fwrite(fileName, imgData)
    
    if OpenTheFile:
        try:
            print(f'>>>> 预览文件.')
            img = cv2.imread(fileName)
            cv2.imshow(DirectoryName, img)
            cv2.waitKey(0)
        except cv2.error:
            print(f'>>>> 非图片文件，不采取预览的操作。\n>>>> fileName: {fileName}')
            ctypes.windll.shell32.ShellExecuteW(None, 'open', fileName.replace('/','\\'), None, None, 1)

# 以二进制格式写入文件
def fwrite(filePath :str, fileData :bytes):
    with open(filePath, 'wb') as f:
        f.write(fileData)

# 以二进制格式读取文件
def fread(filePath :str):
    with open(filePath, 'rb') as f:
        return f.read()

# 将json数据保存至文件
def fwriteJson(filePath :str, fileData :bytes):
    fwrite(filePath, jdumps(fileData).encode())

# 将dict类型变量转存为str类型(不对数据转码)
def jdumps(data :dict):
    return dumps(data, ensure_ascii=False)

# 判断目录是否存在，不存在就创建
def CheckDirectory(name):
    if not exists(name):
        mkdir(name)

# 用Pixiv的下载链接命名文件
def SetFileName(url :str):
    fileName = re.findall( # 为文件命名
        r'\w+://[a-zA-Z0-9.\-\_]+/[a-zA-Z\-\_]+/img/'
        r'([0-9a-zA-Z./\_]+)', url, re.S | re.I)[0]
    fileName = fileName.replace('/', '_')
    return fileName

# 一堆jpg转mp4
def jpg2mp4(in_path :List[str], out_path :str, fps :int = 15):
    img_array = []
    for filename in in_path:
        img = cv2.imread(filename)
        height, width, layers = img.shape
        size = (width, height)
        img_array.append(img)
    out = cv2.VideoWriter(out_path, cv2.VideoWriter_fourcc(*'DIVX'), fps, size)
    for i in range(len(img_array)):
        out.write(img_array[i])
    out.release()

def zipProcess(link, fileArchivePath :str, content :Union[ZipFile, bytes], folder :str):
    fwrite(fileArchivePath, content)
    zipData = ZipFile(fileArchivePath, 'r')
    zipName = zipData.namelist()

    jpgToGifTempArchivePath = f'{fileArchivePath}_temp/'
    if not exists(jpgToGifTempArchivePath):
        mkdir(jpgToGifTempArchivePath)
    zipData.extractall(jpgToGifTempArchivePath)
    zipData.close()
    
    jpgPath = [f'{jpgToGifTempArchivePath}/{fn}' for fn in zipName]
    gifFileName = SetFileName(link).replace('zip', 'mp4')
    
    jpg2mp4(jpgPath, f'{folder}/{gifFileName}')
    for fn in jpgPath:
        remove(fn)
    remove(fileArchivePath)
    rmdir(jpgToGifTempArchivePath)

def executeMultithreading(function :object, totalNumberOfThreads :int):
    TotalThread = [threading.Thread(target = function, args = (threadNumber, ))
        for threadNumber in range(totalNumberOfThreads)]
    for singleThread in TotalThread: singleThread.start()
    for singleThread in TotalThread: singleThread.join()
# Utils.py End ##########################################################


# request.py Begin ######################################################
def requestGET(self, url :str, timeout :float= 12.0):
    try:
        return rget(url, headers = self.DEFINED_Headers, proxies = self.DEFINED_Proxy, timeout = timeout)
    except:
        import os, sys
        print(f'---> Thread: 链接获取发生错误，请检查网络情况.')
        try: sys.exit(1)
        except:
            try: os._exit(1)
            except: exit(1)

def initializeDownload(self, folder :str):
    self.DEFINED_Headers['Referer'] = Referer
    CheckDirectory(folder)
    print(f'>>>> 开始下载作品.')

class old_pixiv:
    def __init__(self, HTTP_Cookie :str, Proxy :dict = None):
        self.DEFINED_Cookie       = HTTP_Cookie # 你在Pixiv网站上的Cookie
        self.DEFINED_Proxy        = Proxy       # 代理服务器
        self.DEFINED_Headers      = {"User-Agent": UserAgent, "Cookie": self.DEFINED_Cookie}
        self.totalNumberOfThreads = 8  # 多线程下载时使用的线程总数
        self.RESULTS_ArtistInfo   = {} # Json数据，用来存放关注列表中所有用户的ID和名字
        self.RESULTS_ArtworkLinks = [] # List数据，用来存放某位画师所有的作品链接
        self.RESULTS_PictureLinks = [] # List数据，用来存放某个作品链接中的所有图片链接

    def GetArtistInfo(self, YourOwnID :Union[str, int]):
        serialNumber = 1
        for page in range(0, 240001, 24):
            getTotalArtistAPI = \
                f'https://www.pixiv.net/ajax/user/{YourOwnID}/following?offset={page}&limit=24&rest=show'
            TotalArtistID = requestGET(self, getTotalArtistAPI).json()
            if not TotalArtistID['body']['users']:
                print('>>>> Done.'); break
            
            for index in TotalArtistID['body']['users']:
                print(f">>>> 已获取{CYAN}{serialNumber:>4}{RESET}个ID")
                self.RESULTS_ArtistInfo[f'{serialNumber:0>4}'] = {
                    "ArtistUrl": f'https://www.pixiv.net/users/{index["userId"]}',
                    "userName" : index['userName']
                }
                serialNumber += 1

    def MultiThreadGetArtworks(self, artistID :Union[str, int]):
        print(f'>>>> 开始获取{artistID}的作品')
        getAllArtworkAPI = f'https://www.pixiv.net/ajax/user/{artistID}/profile/all?lang=zh'
        artworksIDList = [*requestGET(self, getAllArtworkAPI).json()['body']['illusts'].keys()]
        totalArtworks = len(artworksIDList)
        
        self.RESULTS_ArtworkLinks = [f'https://www.pixiv.net/artworks/{ID}' for ID in artworksIDList]
        print(f">>>> 已获取{CYAN}{totalArtworks:>4}{RESET}个作品链接.")
        
        def _getartwork(threadNumber):
            for ID in range(threadNumber, len(artworksIDList), self.totalNumberOfThreads):
                print(f'>>>> Thread {threadNumber:0>2}: 正在获取下载链接.')
                dynamicGraphList  = requestGET(self,
                    f'https://www.pixiv.net/ajax/illust/{artworksIDList[ID]}/ugoira_meta?lang=zh').json()['body']
                staticDiagramList = requestGET(self,
                    f'https://www.pixiv.net/ajax/illust/{artworksIDList[ID]}/pages?lang=zh').json()['body']
                if dynamicGraphList:
                    self.RESULTS_PictureLinks.append(dynamicGraphList['originalSrc'])
                else:
                    for index in staticDiagramList:
                        self.RESULTS_PictureLinks.append(index['urls']['original'])
        executeMultithreading(_getartwork, self.totalNumberOfThreads)
        print(f">>>> 已获取{CYAN}{len(self.RESULTS_PictureLinks):>4}{RESET}个作品下载链接.")

    def MultiThreadDownloadArtwork(self, artworkUrl :List[str], folder :str, zipToMp4 :bool = True):
        initializeDownload(self, folder) # 初始化下载功能
        def _download(threadNumber):
            for artworkIndex in range(threadNumber, len(artworkUrl), self.totalNumberOfThreads):
                fileArchivePath = f'{folder}/{SetFileName(artworkUrl[artworkIndex])}'
                if exists(fileArchivePath) or exists(fileArchivePath.replace('zip', 'mp4')):
                    print(f'>>>> {fileArchivePath}已存在，跳过...')
                    continue
                artworkContent = requestGET(self, artworkUrl[artworkIndex])
                if artworkContent.headers['Content-Type'] == 'application/zip':
                    if zipToMp4:
                        zipProcess(artworkUrl[artworkIndex], fileArchivePath, artworkContent.content, folder)
                    else:
                        fwrite(fileArchivePath, artworkContent.content)
                elif artworkContent.headers['Content-Type'] == 'image/jpeg' or 'image/png':
                    fwrite(fileArchivePath, artworkContent.content)
                print(f'>>>> Thread {threadNumber:0>2}: {CYAN}{artworkUrl[artworkIndex]}{RESET} saved.')
        executeMultithreading(_download, self.totalNumberOfThreads)
# request.py End ######################################################

def fread(path :str, mode :str = 'r', encoding = 'UTF-8') -> Union[str, bytes]:
    with open(path, mode, encoding = encoding) as f:
        return f.read()

def fwrite(path :str, content :Union[str, bytes], mode :str = 'w', encoding = 'UTF-8') -> bool:
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
            'Referer': f'https://www.pixiv.net/users/{self.myself_id}/following',
            'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/116.0'
        }

    def http_get(self, url :str):
        return requests.get(url, headers = self.headers, proxies = self.proxies)

    # 获取指定页码中所有作者ID
    def getArtistList(self, page :int, offset :int) -> list:
        res = self.http_get(f'https://www.pixiv.net/ajax/user/{self.myself_id}/following'
            f'?offset={page*offset}&limit={offset}&rest=show').json()

        if not res['body']['users']:
            return False

        return [index['userId'] for index in res['body']['users']]

    # 获取指定作者主页中所有的作品ID
    def getArtworksIllusts(self, artistID :Union[str, int]):
        url = f'https://www.pixiv.net/ajax/user/{artistID}/profile/all?lang=zh'

        res = self.http_get(url).json()['body']['illusts']

        return [*res.keys()]

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

myself_id = 38279179
artist_id = 58131017
artwork_id = 114304146

px = pixiv(38279179, cookies_path='./pixiv_cookie.txt')
res = px.getIllustsImagesLink(artwork_id)

print(res)
