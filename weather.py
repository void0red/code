#!/usr/bin/env python
# coding=utf-8
from bs4 import BeautifulSoup
import urllib.request as request
import re
from aip import AipSpeech


def makeSoup(url):
    html = request.urlopen(url)
    wstr = ''
    soup = BeautifulSoup(html, 'html.parser')
    soup1 = soup.find_all('li', attrs={'class': 'on'})[1]
    str1 = re.findall(r'>(.*)</', str(soup1))
    b = ''
    try:
        slist = re.findall(r'^(.*)</span>(.*)<i>(.*)$', str1[4])
        for x in range(len(slist[0])):
            b += slist[0][x]
    except:
        b = str1[4]
    if '/' in b:
        b = b.replace('/', '-')
    str1[0] = '下面播报' + str1[0] + '的天气情况: '
    str1[4] = ', 平均温度' + b
    str1[6] = ', 风是' + str1[6]
    for i in str1:
        if i != '':
            wstr = wstr + i
    if '雨' in wstr:
        wstr += '   今天别忘记带雨伞哦！'
    return wstr


def string2mp3(string):
    AppID = '10731026'
    APIKey = 'LYKaBjGHwaBLiymTZytFCam5'
    SecretKey = 'jxudo19Cq5gn4THF9kX1BmxXkGItv3hs'
    aip = AipSpeech(AppID, APIKey, SecretKey)
    result = aip.synthesis(string, 'zh', '1')
    with open('/home/pi/project/weather/temp.mp3', 'wb') as f:
        f.write(result)


def main():
    dongtai = 'http://www.weather.com.cn/weather/101190707.shtml'
    string = makeSoup(dongtai)
    string2mp3(string)
    print(string)


if __name__ == '__main__':
    main()
