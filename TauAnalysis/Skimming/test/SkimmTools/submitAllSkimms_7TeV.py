#!/usr/bin/env python

import os
import re
import commands

#########################################
def prepareCrabCfg(fileName, datasetpath, pset, storage_element, user_remote_dir, publish_data_name):

    os.system("mkdir -p ./"+datasetpath )
    os.system("cp -r Skimms ./"+datasetpath )
    os.system("cp "+fileName+" ./"+datasetpath )
    file = open(fileName, "r") #Opens the file in read-mode
    text = file.read() #Reads the file and assigns the value to a variable
    psetFullPath = commands.getstatusoutput("pwd")[1] + "/" + datasetpath + "/"
    file.close() #Closes the file (read session)
    file = open("./"+datasetpath+"/Tmp_"+fileName, "w") #Opens the file again, this time in write-mode
    text1 = text.replace("datasetpath =","datasetpath = "+datasetpath)
    text2 = text1.replace("pset =","pset = "+ psetFullPath + pset)
    text3 = text2.replace("storage_element =","storage_element = "+storage_element)
    text4 = text3.replace("user_remote_dir =","user_remote_dir = "+user_remote_dir)
    text5 = text4.replace("publish_data_name =","publish_data_name =  "+publish_data_name)
    #validationFile = "muTauValPlots.root"
    #text6 = text5.replace("output_file =","output_file =   "+validationFile)
    #file.write(text6)
    file.write(text5)
    file.close() #Closes the file (write session)
    os.system("cd ./"+datasetpath+"; crab -create -submit -cfg Tmp_"+fileName+"; cd -")
#########################################
#########################################
datasets = {#"/ZJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            #"/WJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_UK_London_IC",
            #"/TTbarJets_Tauola-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_US_Wisconsin",
            ###############
            #"/Ztautau/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            #"/Wtaunu/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            #"/Zee/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            #"/Zmumu/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            ###############
            #"/InclusiveMu15/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_US_Wisconsin",
            #"/ppMuX/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_US_Wisconsin",
            ###############
            #"/PhotonJet_Pt120to170/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            #"/PhotonJet_Pt80to120/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            #"/PhotonJet_Pt170to300/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            #"/PhotonJet_Pt15to20/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            #"/PhotonJet_Pt0to15/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            #"/PhotonJet_Pt30to50/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            #"/PhotonJet_Pt50to80/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            #"/PhotonJet_Pt20to30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Pisa",
            ###############
            #"/QCD_EMEnriched_Pt20to30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_IT_Bari",
            #"/QCD_EMEnriched_Pt30to80/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_US_Wisconsin",
            #"/QCD_EMEnriched_Pt80to170/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_US_Wisconsin",
            #"/QCD_BCtoE_Pt20to30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_US_Wisconsin",
            #"/QCD_BCtoE_Pt80to170/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_US_Wisconsin",
            #"/QCD_BCtoE_Pt30to80/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO":"T2_US_Wisconsin",
            ################
            #"/QCD_Pt-20_MuEnrichedPt10_7TeV-pythia6/Spring10-START3X_V26-v1/GEN-SIM-RECO":"T2_UK_London_IC"
            }


for key in datasets.keys():
    prepareCrabCfg("crabDataPublishing.cfg",
                   key,
                   "Skimms/skimManyChannels_cfg.py",
                   datasets[key],
                   "/store/group/ewk/ZtautauSkim",
                   "SkimTauTau_356_pass1")






