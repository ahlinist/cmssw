#! /usr/bin/env python
import os
import string
import re

################################################
### User Section

## setup the output dir and input files
# output = "MC7TeV_MinBias-Summer09-START3X_V25"
# inputlist = "inputMC7TeV_MinBias-Summer09-START3X_V25_short.txt"
output = "MC7TeV_MinBias-START3X_V25B"
inputlist = "inputMC7TeV_MinBias-START3X_V25B-v1_short.txt"

settingfile = "config/cutTable_METNoteOnDec09Data.txt"
# choose among cmst3 8nm 1nh 8nh 1nd 1nw
queue = "1nh"
ijobmax = 25

### End of User Section
################################################

################################################
pwd = os.getcwd()

outputmain = output
if not re.search("^/", outputmain):
    outputmain = pwd + "/" + outputmain

if not re.search("^/", inputlist):
    inputlist = pwd + "/" + inputlist

if not re.search("^/", settingfile):
    settingfile = pwd + "/" + settingfile
################################################
# write on local disk
################################################
os.system("mkdir -p "+outputmain)
os.system("mkdir -p "+outputmain+"/log/")
os.system("mkdir -p "+outputmain+"/input/")
os.system("mkdir -p "+outputmain+"/src/")
os.system("mkdir -p "+outputmain+"/root/")
#################################################
# output prefix
outputPrefix = string.split(outputmain,"/")[-1]
#################################################
numfiles = len(file(inputlist).readlines())

if ijobmax > numfiles:
    ijobmax=numfiles
filesperjob = int(numfiles/ijobmax)
if numfiles%ijobmax!=0:
    filesperjob = filesperjob+1
    ijobmax = int(numfiles/filesperjob)
    if numfiles%filesperjob!=0:
        ijobmax = ijobmax+1
#################################################
input = open(inputlist)
#################################################
for ijob in range(ijobmax):
    # prepare the list file
    inputfilename = outputmain+"/input/input_"+str(ijob)+".list"
    inputfile = open(inputfilename,"w")
    for i in range(filesperjob):
        line = input.readline()
        if line != "":
            inputfile.write(line)
        continue
    inputfile.close()

    # prepare the script to run
    outputname = outputmain+"/src/submit_"+str(ijob)+".src"
    outputfile = open(outputname,"w")
    outputfile.write("#!/bin/bash\n")
    outputfile.write("cd "+pwd+"\n")
    outputfile.write("./main "+inputfilename+" "+settingfile+" "+"promptanaTree/tree"+" "+outputmain+"/root/"+outputPrefix+"_"+str(ijob)+" "+outputmain+"/log/"+outputPrefix+"_"+str(ijob)+"\n")
    outputfile.close
    print    ("bsub -q "+queue+" -o "+outputmain+"/log/"+outputPrefix+"_"+str(ijob)+".log source "+outputname)
    os.system("bsub -q "+queue+" -o "+outputmain+"/log/"+outputPrefix+"_"+str(ijob)+".log source "+outputname)
