```text
===================================Debian Source===============================
deb http://mirrors.cloud.aliyuncs.com/debian/ buster main contrib non-free
deb http://mirrors.cloud.aliyuncs.com/debian/ buster-updates main contrib non-free
deb http://mirrors.cloud.aliyuncs.com/debian/ buster-proposed-updates main non-free contrib
deb http://mirrors.cloud.aliyuncs.com/debian/ buster-backports main non-free contrib
deb-src http://mirrors.cloud.aliyuncs.com/debian/ buster-updates main contrib non-free
deb-src http://mirrors.cloud.aliyuncs.com/debian/ buster main contrib non-free
deb-src http://mirrors.cloud.aliyuncs.com/debian/ buster-proposed-updates main contrib non-free
deb-src http://mirrors.cloud.aliyuncs.com/debian/ buster-backports main contrib non-free
deb http://mirrors.cloud.aliyuncs.com/debian-security/ buster/updates main non-free contrib
deb-src http://mirrors.cloud.aliyuncs.com/debian-security/ buster/updates main non-free contrib

deb http://mirrors.tuna.tsinghua.edu.cn/debian/ buster main contrib non-free
deb http://mirrors.tuna.tsinghua.edu.cn/debian/ buster-updates main contrib non-free
deb http://mirrors.tuna.tsinghua.edu.cn/debian/ buster-backports main contrib non-free
deb http://mirrors.tuna.tsinghua.edu.cn/debian-security buster/updates main contrib non-free

#RubyGem
deb http://ftp.debian.org/debian/ experimental main contrib non-free

===================================Ubuntu Source===============================
# 18.04
# TUNA
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-updates main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-backports main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-security main restricted universe multiverse

# USTC
deb http://mirrors.ustc.edu.cn/ubuntu/ bionic main restricted universe multiverse
deb http://mirrors.ustc.edu.cn/ubuntu/ bionic-security main restricted universe multiverse
deb http://mirrors.ustc.edu.cn/ubuntu/ bionic-updates main restricted universe multiverse
deb http://mirrors.ustc.edu.cn/ubuntu/ bionic-backports main restricted universe multiverse

# 20.04
# TUNA
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ focal main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ focal-updates main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ focal-backports main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-security main restricted universe multiverse

# USTC
deb http://mirrors.ustc.edu.cn/ubuntu/ focal main restricted universe multiverse
deb http://mirrors.ustc.edu.cn/ubuntu/ focal-security main restricted universe multiverse
deb http://mirrors.ustc.edu.cn/ubuntu/ focal-updates main restricted universe multiverse
deb http://mirrors.ustc.edu.cn/ubuntu/ focal-backports main restricted universe multiverse

===================================Parrot Source===============================
deb http://mirrors.ustc.edu.cn/parrot rolling main contrib non-free
deb http://mirrors.ustc.edu.cn/parrot rolling-security main contrib non-free

deb http://mirrors.tuna.tsinghua.edu.cn/parrot rolling main contrib non-free
deb http://mirrors.tuna.tsinghua.edu.cn/parrot rolling-security main contrib non-free


===================================Kali Source=================================
# ustc 中科大
deb http://mirrors.ustc.edu.cn/kali kali-rolling main non-free contrib
deb-src http://mirrors.ustc.edu.cn/kali kali-rolling main non-free contrib

# tuna 清华源
deb http://mirrors.tuna.tsinghua.edu.cn/kali kali-rolling main contrib non-free

===================================Linux Program===============================

# 在Ubuntu或Debian上安装Wpscan的步骤
apt install ruby
apt install build-essential libcurl4-openssl-dev libxml2 libxml2-dev libxslt1-dev ruby-dev  libgmp-dev zlib1g-dev
gem install wpscan

# 为保证系统功能齐全并且稳定运行所需的软件与工具包
apt install nmap whois htop wapiti nikto ufw libpcre3 libpcre3-dev sqlmap zlib1g zlib1g-dev libdnet \
  libdnet-dev gcc g++ make automake zip whatweb wget rar zip perl net-tools dnsutils libssl-dev libssl1.1 \
  libssl3 build-essential python-dev python-setuptools python-pip python-smbus libncursesw5-dev \
  libgdbm-dev libc6-dev zlib1g-dev libsqlite3-dev tk-dev libssl-dev openssl libffi-dev git vim curl
apt install bless ghex fcitx-*

# Metasploit - msfconsole
curl https://raw.githubusercontent.com/rapid7/metasploit-omnibus/master/config/templates/metasploit-framework-wrappers/msfupdate.erb > msfinstall && \
  chmod 755 msfinstall && \
  ./msfinstall

====================================Py-PIP====================================
./configure --enable-optimizations --prefix=/usr/local/python3
make && make install

#清华源
python -m pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple

#各种库
python -m pip install \
    requests six pillow qrcode paramiko colorama pyinstaller pysocks numpy scipy lxml \
    scrapy flask django pycryptodome rsa hexdump opencv-python


====================================阿里云====================================
域名解析

sngrotesque.com
主机记录	记录类型	解析线路	记录值
_dnsauth	TXT	默认	202108270000003qtrppcmh9hex5a8o2mn6mxr3builx6m1j76ph75jef524klbh
@	A	默认	ip地址
www	A	默认	ip地址

lolibt.top
www	A	默认	ip地址
@	A	默认	ip地址

```