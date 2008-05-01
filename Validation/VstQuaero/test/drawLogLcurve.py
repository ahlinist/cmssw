#!/usr/bin/python

import os,sys,string
import re

def main():
    urlBase = "http://mit.fnal.gov/~knuteson/Quaero/quaero_development/requests/"
    for n in range(170):
        requestNumber=3353+n
#        requestNumber = 3176+n, 120
        mass = 50+10*n
        x = 100+25*(n%5)
        yy = [ 100, 120, 130, 140, 150, 175, 200 ]
        y = 500+500*(n/5)
        result = os.popen("links -source "+urlBase+str(requestNumber)+"/answer.txt | grep failed").read()
        if(result==""):
            result = os.popen("links -source "+urlBase+str(requestNumber)+"/answer.txt | grep log10").read()
        else:
            result = "failed"
        logL = string.strip(string.split(result," ")[-1])
        print requestNumber,"\t",logL
    return 0

main()
