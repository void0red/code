import time
from selenium import webdriver
import urllib.request
import urllib.parse
import re

# qq = 'userid'
# pwd = 'password' 
# send = 'content'


#g_tk
def g_tk(skey):
	hash = 5381
	for i in range(len(skey)):
		hash += hash*(2**5) + ord(skey[i].encode('UTF-8'))
	return hash & 2147483647

#login_QQ
def login(qq,pwd):
	driver = webdriver.Firefox()
	driver.maximize_window()
	driver.get("http://i.qq.com/")
	try:
	    driver.find_element_by_id('login_div')
	    a = True
	except:
	    a = False

	if a == True:
	    driver.switch_to.frame('login_frame')
	    driver.find_element_by_id('switcher_plogin').click()
	    driver.find_element_by_id('u').clear()
	    driver.find_element_by_id('u').send_keys(qq)
	    driver.find_element_by_id('p').clear()
	    driver.find_element_by_id('p').send_keys(pwd)
	    driver.find_element_by_id('login_button').click()
	    time.sleep(20)
	    driver.switch_to_default_content()
	    page = driver.page_source
	    recv = driver.get_cookies()
	    driver.close()
	return(recv,page)

(recv,page) = login(qq,pwd)

result = re.search(r'window\.g_qzonetoken = \(function\(\)\{ try\{return(.+?)catch',page).group()

qzonetoken = re.search(r'"[1-9a-z](.+?)"',result).group()
qzonetoken = qzonetoken.lstrip('"').rstrip('"')

cookies =''

for x in recv:
	cookies += x['name'] + '=' + x['value'] + ';'

	if x['name'] == 'p_skey':
		p_skey = x['value']

g_tk = g_tk(p_skey)

post_params = {
		'qzreferrer':r'https://user.qzone.qq.com/' + qq,
		'syn_tweet_verson':'1',
		'paramstr':'1',
		'pic_template':'',
		'richtype':'',
		'richval':'',
		'special_url':'',
		'subrichtype':'',
		'who':'1',
		'con':send,
		'feedversion':'1',
		'ver':'1',
		'ugc_right':'1',
		'to_sign':'0',
		'hostuin':qq,
		'code_version':'1',
		'format':'fs'
}


post_params = urllib.parse.urlencode(post_params).encode('utf-8')

length = len(post_params)

post_headers = {
		'Host':r'user.qzone.qq.com',
		'User-Agent':r'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:56.0) Gecko/20100101 Firefox/56.0',
		'Accept':r'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
		'Accept-Language':r'zh-CN,en-US;q=0.7,en;q=0.3',
		'Accept-Encoding':r'gzip, deflate, br',
		'Content-Type':r'application/x-www-form-urlencoded',
		'Content-Length':length,
		'Cookie':cookies,
		'DNT':'1',
		'Connection':'keep-alive',
		'Upgrade-Insecure-Requests':'1'
}
	
post_url = r'https://user.qzone.qq.com/proxy/domain/taotao.qzone.qq.com/cgi-bin/emotion_cgi_publish_v6'
post_url += '?g_tk=' + str(g_tk) + '&qzonetoken=' + str(qzonetoken)


r = urllib.request.Request(post_url,data=post_params,headers=post_headers)

urllib.request.urlopen(r)
