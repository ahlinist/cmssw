#!/usr/bin/env python

import os
import re
import commands

#########################################
def prepareCrabCfg(fileName, datasetpath, pset, storage_element, user_remote_dir, publish_data_name):

    os.system("mkdir -p ./"+aDataSet )
    os.system("cp -r Skimms ./"+aDataSet )
    os.system("cp "+fileName+" ./"+aDataSet )
    file = open(fileName, "r") #Opens the file in read-mode
    text = file.read() #Reads the file and assigns the value to a variable
    psetFullPath = commands.getstatusoutput("pwd")[1] + "/" + aDataSet + "/"
    file.close() #Closes the file (read session)
    file = open("./"+aDataSet+"/Tmp_"+fileName, "w") #Opens the file again, this time in write-mode
    text1 = text.replace("datasetpath =","datasetpath = "+datasetpath)
    text2 = text1.replace("pset =","pset = "+ psetFullPath + pset)
    text3 = text2.replace("storage_element =","storage_element = "+storage_element)
    text4 = text3.replace("user_remote_dir =","user_remote_dir = "+user_remote_dir)
    text5 = text4.replace("publish_data_name =","publish_data_name =  "+publish_data_name)
    #validationFile = ""
    #text6 = text5.replace("output_file = ","output_file =   "+validationFile)
    file.write(text5)
    file.close() #Closes the file (write session)
    os.system("cd ./"+aDataSet+"; crab -create -submit -cfg Tmp_"+fileName+"; cd -")
#########################################
#########################################
datasets = ["/ZJets-madgraph/Summer09-MC_31X_V3_7TeV-v2/GEN-SIM-RECO",
            "/WJets-madgraph/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO",
            "/InclusiveMu15/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO"
            ]

datasets = ["/VqqJets-madgraph/Summer09-MC_31X_V3_7TeV_preproduction-v2/GEN-SIM-RECO"]

datasets = ["/QCD_Pt15_BiasedTau/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO",
            "/Ztautau/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO",
            "/Zee/Summer09-MC_31X_V3_7TeV_TrackingParticles-v1/GEN-SIM-RECO",
            "/TTbar/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO"]

for aDataSet in datasets:
    '''
#### Mu-Tau jet skimm. Output goes to T2_IT_Bari    
    prepareCrabCfg("crabDataPublishing.cfg",
                   aDataSet,
                   "Skimms/skimMuTau_cfg.py",
                   "T2_IT_Bari",
                   "ZtautauSkimMuTau",
                   "SkimMuTau_7TeV_314_pass4")
#### El-Tau jet skimm. Output goes to T2_IT_Pisa
    prepareCrabCfg("crabDataPublishing.cfg",
                   aDataSet,
                   "Skimms/skimElecTau_cfg.py",
                   "T2_IT_Pisa",
                   "ZtautauSkimElTau",
                   "SkimElTau_7TeV_314_pass1")
                   
#### El-Mu jet skimm. Output goes to T2_US_Wisconsin
    prepareCrabCfg("crabDataPublishing.cfg",
                   aDataSet,
                   "Skimms/skimElecMu_cfg.py",
                   "T2_US_Wisconsin",
                   "ZtautauSkimElMu",
                   "SkimElMu_7TeV_314_pass3")
    '''
#### Tau-Tau jet skimm. Output goes to T2_IT_Pisa
    prepareCrabCfg("crabDataPublishing.cfg",
                   aDataSet,
                   "Skimms/skimDiTau_cfg.py",
                   "T2_IT_Pisa",
                   "ZtautauSkimTauTau",
                   "SkimTauTau_7TeV_314_pass4")    

'''
    prepareCrabCfg("crabDataPublishing.cfg",
                   aDataSet,
                   "Skimms/skimMuTau_cfg.py",
                   "T2_FR_GRIF_LLR",
                   "ZtautauSkimMuTau",
                   "SkimMuTau_314_pass1")

'''
