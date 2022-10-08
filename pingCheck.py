#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#有上面一條後才能在在註釋上用中文
from globalVar import gloVar
import threading
import time
import sys
import os
import myPing
import logging
logging.debug("pingCheck Start")

import myConfigParser
pingFile = sys.path[0] + "/data/pingResult.conf"
config = myConfigParser.myConfigParser()

pingServerResultList = [False,False]
pingFaileCount = 0

class pingCheck(threading.Thread):
	def __init__(self,threadID,threadName,delay):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.threadName = threadName
		self.delay = delay
		
	def run(self):
		while 1:
			global pingFaileCount
			reServer = myPing.myPing(gloVar.G_serverIp,"result")
			pingServerResultList[0] = pingServerResultList[1]
			pingServerResultList[1] = reServer.getPingResult()
			if (not pingServerResultList[0]) and (not pingServerResultList[1]):
				gloVar.G_pingServerResult = False
				pingFaileCount+=1
				if pingFaileCount > 13:#连续13次ping失败重启
					logging.debug("reboot")
					os.system("reboot")
			else:
				gloVar.G_pingServerResult = True
				pingFaileCount = 0
			#logging.debug(gloVar.G_pingServerResult)
			config.read(pingFile)
			#先将原配置数据保存下来
			for sections in config.sections():
				for items in config.items(sections):
					config.set(sections,items[0],items[1])
			#key1的section不存在就新建一个
			if "key1" not in config.sections():
				config.add_section("key1")
			#数据有变化的另再修改下
			config.set("key1","pingServerResult",gloVar.G_pingServerResult)#给section中option赋值（新增，或者是修改）
			#再写入到文件
			with open(pingFile,"w+") as f:
				config.write(f)
				
			time.sleep(self.delay)
thread1 = pingCheck(3,"pingThread",3)
thread1.start()
