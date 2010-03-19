#! /usr/bin/env python
import os

## setup the output dir and input files
output = "MC900_test"
inputlist = "inputMC900GeV.txt"

settingfile = "config/cutFileExample.txt"
# choose among cmst3 8nm 1nh 8nh 1nd 1nw 
queue = "1nh"
ijobmax = 35
################################################
# to write on local disks
################################################
outputmain = output

os.system("mkdir "+output)
os.system("mkdir "+output+"/log/")
os.system("mkdir "+output+"/input/")
os.system("mkdir "+output+"/src/")
outputroot = outputmain+"/root/"
os.system("mkdir "+outputroot)
################################################
#look for the current directory
################################################
os.system("\\rm tmp.log")
os.system("echo $PWD > tmp.log")
tmp = open("tmp.log")
pwd = tmp.readline()
tmp.close()
os.system("\\rm tmp.log")
#################################################
numfiles = reduce(lambda x,y: x+1, file(inputlist).xreadlines(), 0)
filesperjob = numfiles/ijobmax
filesperjob = filesperjob
extrafiles  = numfiles%ijobmax
input = open(inputlist)
######################################
for ijob in range(ijobmax):
    # prepare the list file
    inputfilename = output+"/input/input_"+str(ijob)+".list"
    inputfile = open(inputfilename,'w')
    # if it is a normal job get filesperjob lines
    if ijob != (ijobmax-1):
        for line in range(filesperjob):
            ntpfile = input.readline()
            if ntpfile != '':
                inputfile.write(ntpfile)
            continue
    else:
        # if it is the last job get ALL remaining lines
        ntpfile = input.readline()
        while ntpfile != '':
            inputfile.write(ntpfile)
            ntpfile = input.readline()
            continue
    inputfile.close()

    # prepare the script to run
    outputname = output+"/src/submit_"+str(ijob)+".src"
    outputfile = open(outputname,'w')
    outputfile.write('#!/bin/bash\n')
    outputfile.write('cd '+pwd)
    outputfile.write('./main '+inputfilename+" "+settingfile+" "+ "promptanaTree/tree" + " "+ outputroot+output+"_"+str(ijob)+" "+output+"/log/cutEfficiencyFile_"+str(ijob)+"\n") 
    outputfile.close
    os.system("echo bsub -q "+queue+" -o "+output+"/log/"+output+"_"+str(ijob)+".log source "+pwd[:-1]+"/"+outputname)
    os.system("bsub -q "+queue+" -o "+output+"/log/"+output+"_"+str(ijob)+".log source "+pwd[:-1]+"/"+outputname)
    ijob = ijob+1
    continue
