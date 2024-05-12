from icmplib import ping

RED    = '\x1b[91m'
GREEN  = '\x1b[92m'
YELLOW = '\x1b[93m'
BLUE   = '\x1b[94m'
RESET  = '\x1b[0m'

print(f'{YELLOW}[?]{RESET} 本代码是用于测试卡拉彼丘游戏延迟，使用ICMP包进行测试，实际延迟请以游戏内为准。')

CalabiYauServer = {
    '重庆': ['111.10.11.73', '111.10.11.250'],
    '南京': ['36.155.164.82', '36.155.202.56'],
    '天津': ['111.30.170.175'],
    '广州': ['120.233.18.175']
}

def test_ping(location :str, ip_address :str):
    print(f'{GREEN}[!]{RESET} 本轮测试服务器定位地址：{RED}{location}{RESET}，服务器IP：{RED}{ip_address}{RESET}')

    res = ping(ip_address, count = 12, interval = 0.05, payload_size = 768)

    return res.avg_rtt, res.max_rtt, res.min_rtt

def main():
    for items in CalabiYauServer:
        for ipaddr in CalabiYauServer[items]:
            res = test_ping(items, ipaddr)
            print(f'{BLUE}[+]{RESET} 平均延迟：{BLUE}{res[0]}{RESET}，最大延迟：{BLUE}{res[1]}{RESET}，最小延迟：{BLUE}{res[2]}{RESET}')

if __name__ == '__main__':
    main()

