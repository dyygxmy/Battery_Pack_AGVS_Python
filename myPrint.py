#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#有上面一條後才能在在註釋上用中文
import logging
import sys
import time
class myPrint:
	def __init__(self):
		pass
	def	writeLog(self):
		logging.basicConfig(level=logging.DEBUG,#控制台打印的日志级别
							filename=sys.path[0]+"/log/" + time.strftime("%Y-%m-%d") + "python.log",#写日志的目录
							filemode='a',#w就是写模式，每次都会重新写日志，覆盖之前的日志 a是追加模式，默认如果不写的话，就是追加模式
							format='[%(asctime)s] %(levelname)s: %(message)s'#日志格式 [日期 时间] [项目名->line:打印所在行]-DEBUG: 打印内容
							)
myPri = myPrint()
myPri.writeLog()
