#! /usr/bin/env python
# -----------------------------------------------------------------------
# Name       : updateHelpLinks.py
# Purpose    : update the help links on quaero.html
# Created    : 22-Aug-2001   Bruce Knuteson
# -----------------------------------------------------------------------
import os,sys,string
import re, regex, regsub
# ______________________________________________________________________
__author__ = 'Bruce Knuteson, knuteson@fnal.gov'
__version__ = '00.00.00'
__filename__ = 'updateHelpLinks.py'
# ______________________________________________________________________

def main():
	metaHtmlFilename = "Quaero/web/quaero_meta.html"
	standardHtmlFilename = "Quaero/web/quaero.html"
	latex2htmlLogFilename = "Quaero/doc/manual/html.log"
	metaHtml = open(metaHtmlFilename,'r')
	standardHtml = open(standardHtmlFilename,'w')
	classified=0
	for line1 in metaHtml.readlines():
		keys = re.search("helpwindow_MANUAL\((.*)\)\.html",line1)
		if(not keys==None):
			key = keys.group(1)
			key = regsub.gsub("\+","\\+",key)
			latex2htmlLog = open(latex2htmlLogFilename,'r')
			foundNodeNumber = 0
			for line2 in latex2htmlLog.readlines():
				nodeNumber = re.search('"'+key+'" for node([0-9]*)\.html',line2)
				if(not nodeNumber==None):
					line1 = regsub.gsub("helpwindow_MANUAL("+key+").html","manual/manual/node"+nodeNumber.group(1)+".html",line1)
					foundNodeNumber = 1
			if(foundNodeNumber==0):
				print 'Key "'+key+'" not found.'
			latex2htmlLog.close()
		if regex.search("BeginClassified",line1) >= 0:
			classified=1
		if regex.search("EndClassified",line1) >= 0:
			classified=0
		if(classified==0):
			standardHtml.write(line1)
		if regex.search("</html>",line1) >= 0:
			break
	metaHtml.close()
	standardHtml.close()
# ______________________________________________________________________


main()
