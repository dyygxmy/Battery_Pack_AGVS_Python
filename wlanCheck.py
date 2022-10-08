#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#有上面一條後才能在在註釋上用中文
import threading
import time
import os
import commands
import logging
logging.debug("wlanCheck Start")

class wlanCheck(threading.Thread):
	def __init__(self,threadID,threadName,delay):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.threadName = threadName
		self.delay = delay
		
	def run(self):
		while 1:
			(ifconfig_status, ifconfig_output) = commands.getstatusoutput("ifconfig")
			if "wlan0" in ifconfig_output:
				pass
				#logging.debug("check wlan0 is up")
			else:
				(wlan0_status, wlan0_output) = commands.getstatusoutput("ifconfig wlan1 up")
				if 0 == wlan1_status:
					logging.debug("check ifconfig wlan0 success")
				else:
					logging.debug("check ifconfig wlan0 faile")
			if "wlan1" in ifconfig_output:
				pass
				# logging.debug("check wlan1 is up")
			else:
				(wlan1_status, wlan1_output) = commands.getstatusoutput("ifconfig wlan1 up")
				if 0 == wlan1_status:
					logging.debug("check ifconfig wlan1 success")
				else:
					logging.debug("check ifconfig wlan1 faile")
			time.sleep(self.delay)
thread5 = wlanCheck(5,"thread5",10)
thread5.start()