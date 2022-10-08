#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#有上面一條後才能在在註釋上用中文

import threading
import commands
import time
import sys
import logging
import myConfigParser
from globalVar import gloVar
logging.debug("scaning Start")

scanFile = sys.path[0] + "/data/scaning.conf"
config = myConfigParser.myConfigParser()

class scaning(threading.Thread):
	def __init__(self,threadID,threadName,delay):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.threadName = threadName
		self.delay = delay
	def run(self):
		while 1:
			(status, output) = commands.getstatusoutput("iwlist wlan1 scan | egrep \"Address:|Signal\ level=|ESSID:\"")
			if 0 == status:
				#logging.debug(output)
				if output.__contains__("BIW-AP"):
					gloVar.G_scanCount += 1
					config.read(scanFile)
					#先将原配置数据保存下来
					for sections in config.sections():
						for items in config.items(sections):
							config.set(sections,items[0],items[1])
			
					#key1的section不存在就新建一个
					if "key1" not in config.sections():
						config.add_section("key1")
				
					#数据有变化的另再修改下
					config.set("key1","cycle",gloVar.G_scanCount)#给section中option赋值（新增，或者是修改）
					config.set("key1","scanData","\"" + output + "\"")
					#再写入到文件
					with open(scanFile,"w+") as f:
						config.write(f)
			else:
				logging.debug("scan faile")
			time.sleep(self.delay)
thread1 = scaning(1,"scanThread",10)
thread1.start()



