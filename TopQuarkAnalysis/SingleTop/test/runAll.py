#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms
import os,sys,re,shutil


#Original config file
f = open("SingleTopSystematics_New_cfg.py")

#Channels to include
channels = [
    "TChannel",
    "TTBar",
#    "WJets",
#    "Data",
    ]





#Implementation:

#Function to replace a sequence of characters channelOld to channelNew in a file 
def changeChannel(fileName,channelOld,channelNew): 
    print " Channel test " + channelNew
    file = open(fileName)
    lines = file.readlines()
    o = open(channelNew+"_cfg.py","w") 
    for line in lines:
        words = line.split()
        for word in words:
            if channelOld in word: 
#                print " line old " + line
                line = line.replace(word,word.replace(channelOld,channelNew))
#                print " line new " + line
        o.write(line)
    o.close()
    return o

#Implementation of the loop part:

#Channel in the original file
startChannel = "TChannel"#channels[0]

tmpName = "temp.py"
shutil.copy("SingleTopSystematics_New_cfg.py",tmpName)

for channel in channels:


    channelOld = startChannel
    
    cfg_file = changeChannel(tmpName,channelOld,channel)
    command = 'nohup cmsRun ./' + channel+'_cfg.py > /tmp/oiorio/'+channel+'.log &'

    print command
    os.system(command ) 
#    os.system("bg") 
#    os.system('rm '+channel+'_cfg.py' ) 

os.system('rm '+tmpName) 
#changeChannel(f,aChannel,startChannel)

#os.system(command)



