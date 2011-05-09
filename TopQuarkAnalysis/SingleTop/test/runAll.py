#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms
import os,sys,re,shutil


#Original config file
fileName = "SingleTopSystematics_cfg.py"
#fileName = "SingleTopSystematics_split_cfg.py"

#Channels to include
channels = [
#    "TChannel",
#    "TTBar",
    "WJets",
#    "Data",
#    "Wc_wc_",
#    "Vqq_wbb_",
#    "Vqq_wcc_",
#    "WJets_wbb_",
#    "WJets_wcc_",
    ]

#Path to take data merged files
dataPath = "file:/tmp/oiorio/"



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
        
    if channel == "Data":#Temporary inelegant solution due to the separation of mu/e: will fix it at some point
        line = "process.source.fileNames = cms.untracked.vstring('"+dataPath+"DataMuMerged.root','"+dataPath+"DataEleMerged.root',)"
        o.write(line)
    o.close()
    return o

#Implementation of the loop part:

#Channel in the original file
startChannel = "TChannel"#channels[0]

f= open(fileName)

tmpName = "temp.py"
shutil.copy(fileName,tmpName)

for channel in channels:


    channelOld = startChannel
    
    cfg_file = changeChannel(tmpName,channelOld,channel)
    command = 'nohup cmsRun ./' + channel+'_cfg.py > /tmp/oiorio/'+channel+'.log &'

    print command
#    os.system(command ) 
#    os.system("bg") 
#    os.system('rm '+channel+'_cfg.py' ) 

os.system('rm '+tmpName) 
#changeChannel(f,aChannel,startChannel)

#os.system(command)



