#!/usr/bin/env python

import os
import re
import commands

#########################################
def prepareCrabCfg(datasetpath, pset, user_remote_dir):

    fileName = "crabSaveToCastor.cfg"
    outputFile = datasetpath.split("-")[1]
    outputFile = outputFile.split("_")[0] + "_Merged.root"
    print "datasetpath: ",datasetpath
    print "outputFile: ", outputFile
    os.system("mkdir -p ./"+aDataSet )    
    command = "sed s/\"muTauSkim.root\"/\""+outputFile+"\"/ skimMuTauDoinNothing_cfg.py > Tmp_skimMuTauDoinNothing_cfg.py"
    os.system(command)

    os.system("mv Tmp_skimMuTauDoinNothing_cfg.py ./"+aDataSet )
    file = open(fileName, "r") #Opens the file in read-mode
    text = file.read() #Reads the file and assigns the value to a variable
    psetFullPath = commands.getstatusoutput("pwd")[1] + "/" + aDataSet + "/"
    file.close() #Closes the file (read session)
    file = open("./"+aDataSet+"/Tmp_"+fileName, "w") #Opens the file again, this time in write-mode
    text1 = text.replace("datasetpath =","datasetpath = "+datasetpath)
    text2 = text1.replace("pset =","pset = "+ psetFullPath + pset)
    text3 = text2.replace("user_remote_dir =","user_remote_dir = "+user_remote_dir)
    file.write(text3)
    file.close() #Closes the file (write session)
    os.system("cd ./"+aDataSet+"; crab -create -submit -cfg Tmp_"+fileName+"; cd -")
#########################################
#########################################    
datasets = ["/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4-445be53e871c3e3ab339c8bec76db9f0/USER",
            "/ZJets-madgraph/akalinow-SkimElMu_7TeV_314_pass3-79e10cf68103c50994df21d46a9dba5c/USER"]


datasets = ["/WJets-madgraph/akalinow-SkimElMu_7TeV_314_pass3-79e10cf68103c50994df21d46a9dba5c/USER"]


datasets = ["/InclusiveMu15/akalinow-SkimElMu_7TeV_314_pass3-79e10cf68103c50994df21d46a9dba5c/USER"]

for aDataSet in datasets:
    user_remote_dir = "/user/a/akalinow/CMS/ZTauTauSkims/7TeV/" + aDataSet.split("/")[1]+"/"+ aDataSet.split("/")[2].split("-")[0] + "-" +aDataSet.split("/")[2].split("-")[1]
    os.system("rfmkdir -p /castor/cern.ch/"+user_remote_dir)
    os.system("rfchmod +775 /castor/cern.ch/"+user_remote_dir)
    prepareCrabCfg(aDataSet,
                   "skimMuTauDoinNothing_cfg.py",
                   user_remote_dir)







