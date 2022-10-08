#!/usr/bin/python
# -*- coding: UTF-8 -*-
import myConfigParser
import sys
class gloVar():
	G_scanCount = 0
	G_iwconfigCount = 0
	G_serverIp = "11.11.11.11"
	G_wirelessNetIp = "11.11.11.11"
	G_netmask = "255.0.0.0"
	G_EthernetPrefix0 = "11.11.11."
	G_EthernetIp0 = "11.11.11.11"
	G_EthernetIp1 = "11.11.11.11"
	
	G_pingServerResult = False
	G_ViNum = ""
	G_wifiIpPrefix = "11.11.11."

	G_test_wifiPath = "/var/test_wifi"
	G_wpaPath = "/agvs/wpa/wpa_supplicant_new.conf"
	G_configIniPath = "/agvs/agvsConfig.ini"
	G_sections1 = "baseinfo"

	config = myConfigParser.myConfigParser()#提取配置文件数据
	config.read(G_configIniPath)
	for sections in config.sections():
		if sections == G_sections1:
			for items in config.items(sections):
				if items[0] == "AGVSServer":
					G_serverIp = items[1]
				if items[0] == "wifiIpPrefix":
					G_wifiIpPrefix = items[1]
				if items[0] == "ViNum":
					G_ViNum = items[1]
				if items[0] == "netmask":
					G_netmask = items[1]
				if items[0] == "EthernetPrefix0":
					G_EthernetPrefix0 = items[1]
					
			G_EthernetIp0 = G_EthernetPrefix0 + str(100+int(G_ViNum))
			G_wirelessNetIp = G_wifiIpPrefix + G_ViNum
				