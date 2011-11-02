#!/usr/bin/env python

from sys import argv
from math import fabs
import os

crab_templ="crab_LOCAL.tmpl"
cfg_templ="dump_42X_local.tmpl"

def readFile(fn,input,lst):
    lst[fn]=[]
    for line in input.split("\n"):
        if ( line != "" ):
            lst[fn].append(line)

def prepareCrab(lst):
    for dataset in lst.keys():
#        print dataset
        dirName=os.path.splitext(os.path.basename(dataset))[0]
#        print dirName
        for file in range(0,len(lst[dataset])):
            cfg='dump_42X_'+ dirName + '_'+str(file)+'.py'
            crab='/crab_'+ dirName + '_'+str(file)+'.cfg'
            f = open(cfg_templ)
            data=f.read()
            data=data.replace('MYINPUTFILENAME',lst[dataset][file])
            data=data.replace('MYOUTFILENAME','output_job'+str(file)+'.root')
            outfile = open(dirName+"/"+cfg,"w")
            outfile.write(data)
            outfile.close()
            
            f = open(crab_templ)
            data=f.read()
            data=data.replace('MYCFGFILE',cfg)
            data=data.replace('MYWORKINGDIR',dirName+'_'+str(file))
            data=data.replace('MYOUTFILE','output_job'+str(file)+'.root')
            data=data.replace('MYOUTDIR',dirName)
            outfile = open(dirName+"/"+crab,"w")
            outfile.write(data)
            outfile.close()
#            print 'crab_'+ dirName + '_'+str(file)+'.cfg

argv.pop(0)
allFiles={}
while (len(argv)>0):
    fn1 = argv.pop(0)
#    print "Reading file " +str(fn1)
    file1 = open(fn1)
    readFile(fn1,file1.read(),allFiles)
    file1.close()

prepareCrab(allFiles)
    
                        

