#!/usr/bin/python

import os,sys,string
import re

def main():
    urlBase = "http://mit.fnal.gov/~knuteson/Quaero/quaero_development/requests/"
    example = sys.argv[1]
    for n in range(int(sys.argv[3])):
        requestNumber=int(sys.argv[2])+n
        result = os.popen("links -source "+urlBase+str(requestNumber)+"/answer.txt | grep failed").read()
        x = 0
        y = 0
        if(result==""):
            result = os.popen("links -source "+urlBase+str(requestNumber)+"/answer.txt | grep log10").read()
            s = os.popen("links -source "+urlBase+str(requestNumber)+"/answer.txt | grep "+example).read()
            s = s.replace("(",",").replace(")",",")
            s = s.split(",")
            if(len(s)>=2):
                x = s[1]
                y = s[2]
            else:
                x = ""
                y = ""
        else:
            result = "failed"
        logL = string.strip(string.split(result," ")[-1])
        print x,y,"\t",logL
    return 0

main()
