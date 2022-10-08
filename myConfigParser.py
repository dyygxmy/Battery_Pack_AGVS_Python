#!/usr/bin/python
# -*- coding: UTF-8 -*-
import ConfigParser

class myConfigParser(ConfigParser.ConfigParser):
	def __init__(self,defaults=None):
		ConfigParser.ConfigParser.__init__(self,defaults=defaults)
	def optionxform(self,optionstr):
		return optionstr
