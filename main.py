#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import commands
from myPrint import myPrint
import logging
import os
logging.debug("myPython main Start")
from globalVar import gloVar

os.system("ifconfig eth0 " + gloVar.G_EthernetIp0)

#开机加载设备、网卡
(rfkill_status, rfkill_output) = commands.getstatusoutput("rfkill unblock all")
if 0 == rfkill_status:
	logging.debug("rfkill success")
	(wlan1_status, wlan1_output) = commands.getstatusoutput("ifconfig wlan1 up")
	if 0 == wlan1_status:
		from scan import scaning#启动scaning线程
		logging.debug("ifconfig wlan1 success")
		(wlan0_status, wlan0_output) = commands.getstatusoutput("ifconfig wlan0 up")
		if 0 == wlan0_status:
			logging.debug("ifconfig wlan0 success")
		else:
			logging.debug("ifconfig wlan0 faile")
	else:
		logging.debug("ifconfig wlan1 faile")
else:
	logging.debug("rfkill faile")

from pingCheck import pingCheck#启动ping线程

from wifiCrtl import wifiCrtl#启动wifi网络管理线程

from iwconfig import wifiState#启动iwconfig线程

from wlanCheck import wlanCheck#启动网卡检测
