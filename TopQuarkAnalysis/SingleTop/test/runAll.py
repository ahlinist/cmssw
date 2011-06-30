#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms
import os,sys,re,shutil


#Original config file
fileName = "SingleTopSystematicsWithTrigger_cfg.py"
#fileName = "SingleTopSystematics_cfg.py"
#fileName = "SingleTopSystematics_split_cfg.py"
#fileName = "SingleTopNEvents_cfg.py"

#Channels to include
channels = [
#
  "DataMu",
  "DataEle",
  "DataMuQCD",
  "DataEleQCD",
#
  "TChannel",
#
  "QCDMu",
  "EMEnriched_30to80",
  "EMEnriched_80to170",
  "BCtoE_20to30",
  "BCtoE_30to80",
  "BCtoE_80to170",
  "GJets_HT_40To100",
  "GJets_HT_100To200",
  "GJets_HT_200",

  "TTBar",
  
   "ZJets",
   "ZJets_wlight",
   "ZJets_wcc",
   "ZJets_wbb",
   "tWChan ",
   "sChan",

  "WJets_wlight",
  "WJets_wcc",
  "WJets_wbb",
  "WJets",
  "Wc_wc",
  "Vqq_wbb",
  "Vqq_wcc",
  "VV",

   ]

#Path to take data merged files
dataPath = "file:/tmp/oiorio/"

#Choose if you want to run or just prepare the configuration files
mode = ""
mode = "cmsRun"


#Use mu , ele or both
channel_instruction = "all"

#Implementation:

#Function to replace a sequence of characters channelOld to channelNew in a file 
def changeChannel(fileName,channelOld,channelNew,switch,isMC): 
    print " Channel test " + channelNew
    channelToReplace = channelNew
    if channelNew=="DataMu" or channelNew == "DataEle" or channelNew == "DataMuQCD" or channelNew =="DataEleQCD":
        channelToReplace = "Data"
        #if channelNew=="DataEle":
       # channelNew_2 = "Data"
    file = open(fileName)
    lines = file.readlines()
    o = open(channelNew+"_cfg.py","w") 
    for line in lines:
        if '"channel_instruction"' in line:
            print line
            line = line.replace('"channel_instruction"','"'+switch+'"')
            print line
        if "MC_instruction" in line and "False" in line:
       #     if "False" in line:
                print line
                line = line.replace("False",isMC)
                print line
        words = line.split()
        for word in words:
            if channelOld in word:  
                #                print " line old " + line
                if (not switch == "all") and ("process.TFileService" in line):
                    line = line.replace(word,word.replace(channelOld,channelNew))
                    print "process.TFileService in line,switch " + switch +" line: \n" +line
                    
                else:
                    line = line.replace(word,word.replace(channelOld,channelToReplace))
                
        o.write(line)   
    #if channel == "Data":#Temporary inelegant solution due to the separation of mu/e: will fix it at some point
        #        line = "process.source.fileNames = cms.untracked.vstring('"+dataPath+"DataMuMerged.root','"+dataPath+"DataEleMerged.root',)"
        #        line = "process.source.fileNames = cms.untracked.vstring('"+dataPath+"DataMuMerged.root',)"
        #       line = "process.source.fileNames = cms.untracked.vstring('"+dataPath+"Mu_v1Merged.root','"+dataPath+"Mu_v2Merged.root','"+dataPath+"Ele_v1Merged.root','"+dataPath+"Ele_v2Merged.root',)"
    if channelNew == "DataMu" or channelNew == "DataMuQCD":
        inputs = "process.source.fileNames = cms.untracked.vstring('"+dataPath+"MuMerged.root',)"
        o.write(inputs)
    if channelNew == "DataEle" or channelNew == "DataEleQCD":
        inputs = "process.source.fileNames = cms.untracked.vstring('"+dataPath+"EleMerged.root',)"
        o.write(inputs)
    o.close()
    return o

#Implementation of the loop part:

#Channel in the original file
startChannel = "TChannel"#channels[0]

f= open(fileName)

tmpName = "temp.py"
shutil.copy(fileName,tmpName)

for channel in channels:

    isMC = "False"
    if channel == "DataMu":
        channel_instruction = "mu"
    elif channel == "DataEle":
        channel_instruction = "ele"
    elif channel == "DataMuQCD":
        channel_instruction = "muqcd"
    elif channel == "DataEleQCD":
        channel_instruction = "eleqcd"
    else : 
        channel_instruction = "allmc"   
        isMC = "True"
    channelOld = startChannel
    
    cfg_file = changeChannel(tmpName,channelOld,channel,channel_instruction,isMC)
    command = 'nohup cmsRun ./' + channel+'_cfg.py > /tmp/oiorio/'+channel+'.log &'
    
    print command

    if mode == "cmsRun":
        os.system(command ) 
#    os.system("bg") 
#    os.system('rm '+channel+'_cfg.py' ) 

os.system('rm '+tmpName) 
#changeChannel(f,aChannel,startChannel)

#os.system(command)



