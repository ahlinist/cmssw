#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms
import os,sys,re,shutil


#Castor directory with all sub-directories:
#inputDir = "/castor/cern.ch/user/o/oiorio/SingleTop/2011/MC2011/NewNtuples"
#inputDir ="/castor/cern.ch/user/o/oiorio/SingleTop/2011/Run2011/NewPatTuples/Ele"
inputDir ="/castor/cern.ch/user/a/acaudron/SingleTop/Triggers"

#Original config file
#fName = "copyTemplate.py"
fName = "copyFlavorSeparationTemplate.py"
f = open(fName)



#Channels to include
channels = [
#    "TChannel",
#    "TTBar",
#    "WJets",
#    "Data",
#    "Vqq",
#    "Wc",
    "SingleMu",
    "TTbar",
    ]


#Prefix = "edmntuple_"
Prefix = ""

Switch = "None"

#Function to replace a sequence of characters channelOld to channelNew in a file 
def changeChannel(fileName,channelOld,channelNew,switch): 
    print " Channel test " + channelNew
    file = open (fileName)
    lines = file.readlines()
    o = open(channelNew+"_cfg.py","w") 
    for line in lines:
        if '"switch_instruction"' in line:
            print line
            line = line.replace('"switch_instruction"','"'+switch+'"')
            print line
        words = line.split()
        for word in words:
            if channelOld in word: 
                #                print " line old " + line
                line = line.replace(word,word.replace(channelOld,channelNew))
                #                print " line new " + line
        o.write(line)
    o.close()    
    return o

#Function to append new input
def appendInput(fileName,directory,channel,prefix): 
    o = open (fileName,"a")

    o.write("process.source.fileNames = cms.untracked.vstring()\n")
    o.write("process.source.fileNames.extend([")
    
    inputRedirect = "rfdir "+directory +"| cut -c68-200 > "+ channel+"_input.py"
    if channel == "Data":
        inputRedirect = "rfdir "+directory +"| cut -c68-200 > "+ channel+"_input.py"
    os.system(inputRedirect)
    tmp = open(channel+"_input.py")
    lines = tmp.readlines()
    for line in lines:
        words = line.split()
        hasWord = "true" 
        for word in words:
            beginName = prefix+channel
            #print "beginName is " + beginName +" word is "+ word
            if beginName in word:
                #print " word ok , is it true? " + hasWord 
                if channel != "Data":
                    line = "'"+line.replace(word,word.replace(beginName,"rfio:"+directory+"/"+beginName))
                else:
                    line = "'"+line.replace(word,word.replace(beginName,"rfio:"+directory+"/"+beginName))
                if ".root" in word:
                    line = line.replace(word,word.replace(".root",".root',"))
#                if "\n" in word:
#                    line = line.replace(word,word.replace("\n",""))
#                print "line after " + line
            else:
                #print ("error! please, check that either the path : \n" +directory +"\n is correct or that the file name contains files starting with '"+beginName+"' \n (try rfdir "+directory +" | grep "+beginName+") \n")
                hasWord = "false" 
        if hasWord == "true":
            o.write(line)
    o.write("])")       
    o.close()    
    #    os.system("rm "+channel+"_input.py")
    return o



#Implementation of the loop part:

#Channel in the original file
startChannel = "TChannel"#channels[0]

tmpName = "temp.py"
shutil.copy(fName,tmpName)

for channel in channels:

    #tmp_file = open(tmpName)

    print "channel is:\n"+ channel
        
    if channel == "WJets":
        Switch = "wlight"
    elif channel == "Vqq":
        Switch = "vqq"
    elif channel == "Wc":
        Switch = "wc"
    else:
        Switch = "None"
    channelOld = startChannel
    
    cfg_file = changeChannel(tmpName,channelOld,channel,Switch)
    
    cfg_file = appendInput("./"+channel+"_cfg.py",inputDir,channel,Prefix)
    
    command = 'nohup cmsRun ./' + channel+'_cfg.py > /tmp/oiorio/'+channel+'_merge.log &'
    
    print command
    os.system(command ) 
    #os.system("rm "+channel+"_cfg.py") 
    
os.system('rm '+tmpName) 
#changeChannel(f,aChannel,startChannel)

#os.system(command)



