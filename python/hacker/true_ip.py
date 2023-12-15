from lxml import etree
import requests

cookie = '_ga=GA1.2.1128415278.1702637659; _gid=GA1.2.1098270206.1702637659; _ga_NVFXGE3YHC=GS1.2.1702637660.1.1.1702638226.0.0.0; search.censys.io.session.id=ayxc4dS36FOKtkRvuOjryMT7rNkSZyeRi8jn_zhP3oL2pI5O0DidSiu87-IUr8vVs-gvvHpRuX_vt8nB1CzLI3qQIguhWhR1jMg8VX72B3AwiSkTkKk10QqYQQkQV3zpbcT_JLuNAQmBTLbgtM0JnQDIH1-vN6WdCDT8Pg; __cf_bm=v77NglhU1vjkXaqhKy2kJ.pawD69.aSX8.hPHMorZt4-1702638885-1-AfWMHayM49xf3cPZ462oB21aUheYcG1XrGaOPw2J21esIpUFiGKPDpqte4NiNlTBd38qp/Yj7MY7NXDGhqGPuAY='

def query(target_url :str):
    url = f'https://search.censys.io/_search?resource=hosts&sort=RELEVANCE&per_page=25&virtual_hosts=EXCLUDE&q={target_url}'
    headers = {
        'Cookie': cookie,
        'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0'
    }

    result = requests.get(url, headers = headers)

    print(result)
    return result

res = query('ys.mihoyo.com')
print(res.text)

