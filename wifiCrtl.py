#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#有上面一條後才能在在註釋上用中文
from globalVar import gloVar
import threading
import time
import os
import logging
import sys
import myConfigParser
logging.debug("wifiCrtl Start")

needSwitchPath = sys.path[0] + "/data/allowSwitch.conf"
config = myConfigParser.myConfigParser()
needSwitch = False

class wifiCrtl(threading.Thread):
	def __init__(self,threadID,threadName,delay):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.threadName = threadName
		self.delay = delay
		
	def run(self):
		global needSwitch
		while 1:
			config.read(needSwitchPath)
			#先将原配置数据保存下来
			for sections in config.sections():
				for items in config.items(sections):
					if items[0] == "needSwitch":
						needSwitch = items[1]
					config.set(sections,items[0],items[1])
			#key1的section不存在就新建一个
			if "key1" not in config.sections():
				config.add_section("key1")
			if needSwitch == str(True):
				needSwitch = False
				config.set("key1","needSwitch",needSwitch)
				with open(needSwitchPath,"w+") as f:
					config.write(f)
				
				os.system("killall wpa_supplicant")
				os.system("rfkill unblock all")
				os.system("ifconfig wlan0 up")
				logging.debug("wpa_supplicant Start")
				os.system("wpa_supplicant -B -P /var/run/wpa_supplicant.wlan0.pid -i wlan0 -c " + gloVar.G_wpaPath + " &")
				logging.debug("wpa_supplicant run")
				ifconfigSetStr = "ifconfig wlan0 " + gloVar.G_wirelessNetIp + " netmask " + gloVar.G_netmask
				#gwSetStr = "route add default gw " + gloVar.G_pingIP + " wlan0"
				os.system(ifconfigSetStr)
				#os.system(gwSetStr)
				myWait = 0
				while myWait < 26:#time out
					if gloVar.G_pingServerResult:
						break
					time.sleep(1)
					myWait += 1
				logging.debug("wpa_supplicant end:"+str(gloVar.G_pingServerResult))
			time.sleep(self.delay)
thread1 = wifiCrtl(2,"thread2",2)
thread1.start()
