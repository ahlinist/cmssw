#!/usr/bin/env python

import os, sys, string, re
from ROOT import *
from array import array

##################################################################################################################################
#### User Section

# Please don't change anything outside the User Section
# Description of different variables is provided below
# More detailed instructions available at https://twiki.cern.ch/twiki/bin/view/CMS/JetMETHistoryDQM

# Turn on/off the run range mode
runRangeMode = True # set to True if you want to turn on the run range mode

# Run range defined as [first,last] (used only if runRangeMode = True)
runRange = [143323,144114]

# List of runs to be processed
#runNumbers = [123596,123615,123732,123815,123818,123908,124009,124020,124023,124024,124025,124027,124030]
#runNumbers = [132440,132442,132471,132473,132474,132476,132477,132478,132569,132572,132596,132597,132598,132599,132601,132602,132605,132606]
###CHANGE
runNumbers = [132601,132716,132662,132661,132659,
132656,132654,132653,132652,132650,132648,132647,
]
#remove also 132478
#runNumbers = [132440,132442,132471,132473,132474,132476,132477,132572,132596,132597,132598,132599,132601,132602,132605,132606]


# Location of the DQM output root files
#fileLocation = "/uscms_data/d2/sturdy07/DQM/Jan29thReReco/900GeV/"
#fileLocation = "/home/xv/weng/CMSSW_3_6_0_pre3/src/Harvesting"
#fileLocation = "/afs/cern.ch/user/w/weng/w1/CMSSW_3_5_6/src/HistDQM"
fileLocation = "/home/hatake/DQM_ROOT_files"
# List of DQM Monitor Elements to be processed
# For each Monitor Element a list of entries is defined with the following meaning:
# Entry 0: Specifies the Monitor Element
# Entry 1: Specifies the quantity to be extracted from the Monitor Element. Can be 'mean' or 'rms'
# Entries 2 and 3: Define the y-axis range. If both numbers are equal, the ROOT default is used
# Entry 4: Monitor Element title
# Entry 5: y-axis title
# Entry 6: File format of the final plot ('png', 'eps', 'pdf', etc.)
dqmMEs = [
  ### CaloMET
  ['MET/CaloMET/BasicCleanup/METTask_CaloMET','mean',-1,-1,'CaloMET_BasicCleanup_METTask_CaloMET_Mean','CaloMET Mean [GeV]','png'],
  ['MET/CaloMET/BasicCleanup/METTask_CaloSumET','mean',-1,-1,'CaloMET_BasicCleanup_METTask_CaloSumET_Mean','CaloSumET Mean [GeV]','png'],

  ['MET/CaloMET/BasicCleanup/METTask_CaloMEx','mean',-1,-1,'CaloMET_BasicCleanup_METTask_CaloMEx_Mean','CaloMEx Mean [GeV]','png'],
 # ['MET/CaloMET/BasicCleanup/METTask_CaloMEx','rms',-1,-1,'CaloMET_BasicCleanup_METTask_CaloMEx_RMS','CaloMEx RMS [GeV]','png'],

  ['MET/CaloMET/BasicCleanup/METTask_CaloMEy','mean',-1,-1,'CaloMET_BasicCleanup_METTask_CaloMEy_Mean','CaloMEy Mean [GeV]','png'],
 # ['MET/CaloMET/BasicCleanup/METTask_CaloMEy','rms',-1,-1,'CaloMET_BasicCleanup_METTask_CaloMEy_RMS','CaloMEy RMS [GeV]','png'],

  ['MET/CaloMET/BasicCleanup/METTask_CaloEmEtFraction','mean',-1,-1,'CaloMET_BasicCleanup_METTask_CaloEmEtFraction_Mean','CaloEmEtFraction Mean ','png'],

  #--- Calo Sum ET from CaloMET block

  ['MET/CaloMET/BasicCleanup/METTask_CaloEmEtInEB','mean',-1,-1,'CaloMET_BasicCleanup_METTask_CaloEmEtInEB_Mean','CaloEmEtInEB Mean [GeV]','png'],
  ['MET/CaloMET/BasicCleanup/METTask_CaloEmEtInEE','mean',-1,-1,'CaloMET_BasicCleanup_METTask_CaloEmEtInEE_Mean','CaloEmEtInEE Mean [GeV]','png'],
  ['MET/CaloMET/BasicCleanup/METTask_CaloEmEtInHF','mean',-1,-1,'CaloMET_BasicCleanup_METTask_CaloEmEtInHF_Mean','CaloEmEtInHF Mean [GeV]','png'],

  ['MET/CaloMET/BasicCleanup/METTask_CaloHadEtInHB','mean',-1,-1,'CaloMET_BasicCleanup_METTask_CaloHadEtInHB_Mean','CaloHadEtInHB Mean [GeV]','png'],
  ['MET/CaloMET/BasicCleanup/METTask_CaloHadEtInHE','mean',-1,-1,'CaloMET_BasicCleanup_METTask_CaloHadEtInHE_Mean','CaloHadEtInHE Mean [GeV]','png'],
  ['MET/CaloMET/BasicCleanup/METTask_CaloHadEtInHF','mean',-1,-1,'CaloMET_BasicCleanup_METTask_CaloHadEtInHF_Mean','CaloHadEtInHF Mean [GeV]','png'],

  #--- 

   ['Jet/CleanedAntiKtJets/Pt','mean',-1,-1,'Jet_CleanedAntiKtJets_Pt2_Mean','Cleaned Jet Pt Mean [GeV]','png'],
   ['Jet/CleanedAntiKtJets/Pt_Barrel','mean',-1,-1,'Jet_CleanedAntiKtJets_Pt_Barrel_Mean','Cleaned Jet Pt Barrel Mean [GeV]','png'],
   ['Jet/CleanedAntiKtJets/Pt_EndCap','mean',-1,-1,'Jet_CleanedAntiKtJets_Pt_Endcap_Mean','Cleaned Jet Pt Endcap Mean [GeV]','png'],
   ['Jet/CleanedAntiKtJets/Pt_Forward','mean',-1,-1,'Jet_CleanedAntiKtJets_Pt_Forward_Mean','Cleaned Jet Pt Forward Mean [GeV]','png'],
  ['Jet/CleanedAntiKtJets/Constituents','mean',-1,-1,'Jet_CleanedAntiKtJets_Constituents_Mean','Cleaned Jet Constituents Mean [GeV]','png'],

##  #---
  
   ['Jet/JPT/Pt','mean',-1,-1,'Jet_JPT_Pt_Mean','Cleaned Jet Pt Mean [GeV]','png'],
   ['Jet/JPT/Phi','mean',-1,-1,'Jet_JPT_Phi_Mean','Cleaned Jet Phi Mean ','png'],
   ['Jet/JPT/Eta','mean',-1,-1,'Jet_JPT_Eta_Mean','Cleaned Jet Eta  Mean ','png'],
   ['Jet/JPT/nTracks','mean',-1,-1,'Jet_JPT_nTracks_Mean','Cleaned Jet nTracks  Mean ','png'],
  
  
## ----
 ['Jet/PFJets/Pt','mean',-1,-1,'Jet_PFJets_Pt_Mean','Cleaned Jet Pt Mean [GeV]','png'],
 ['Jet/PFJets/Phi','mean',-1,-1,'Jet_PFJets_Phi_Mean','Cleaned Jet Phi Mean ','png'],
 ['Jet/PFJets/Eta','mean',-1,-1,'Jet_PFJets_Eta_Mean','Cleaned Jet Eta Mean ','png'],
  
  
  
 #  ['Jet/JPT/Constituents','mean',-1,-1,'Jet_JPT_Constituents_Mean','Cleaned Jet Constituents Mean [GeV]','png'],

  

  #['MET/CaloMET/ExtraCleanup/METTask_CaloMET','mean',-1,-1,'CaloMET_ExtraCleanup_METTask_CaloMET_Mean','CaloMET Mean [GeV]','png'],

  #['MET/CaloMET/ExtraCleanup/METTask_CaloSumET','mean',-1,-1,'CaloMET_ExtraCleanup_METTask_CaloSumET_Mean','CaloSumET Mean [GeV]','png'],

  #['MET/CaloMET/ExtraCleanup/METTask_CaloMEx','mean',-1,-1,'CaloMET_ExtraCleanup_METTask_CaloMEx_Mean','CaloMEx Mean [GeV]','png'],

  #['MET/CaloMET/ExtraCleanup/METTask_CaloMEx','rms',-1,-1,'CaloMET_ExtraCleanup_METTask_CaloMEx_RMS','CaloMEx RMS [GeV]','png'],

  #['MET/CaloMET/ExtraCleanup/METTask_CaloMEy','mean',-1,-1,'CaloMET_ExtraCleanup_METTask_CaloMEy_Mean','CaloMEy Mean [GeV]','png'],

  #['MET/CaloMET/ExtraCleanup/METTask_CaloMEy','rms',-1,-1,'CaloMET_ExtraCleanup_METTask_CaloMEy_RMS','CaloMEy RMS [GeV]','png'],
  #### CaloMETNoHF
  #['MET/CaloMETNoHF/BasicCleanup/METTask_CaloMET','mean',-1,-1,'CaloMETNoHF_BasicCleanup_METTask_CaloMET_Mean','CaloMET Mean [GeV]','png'],

  #['MET/CaloMETNoHF/BasicCleanup/METTask_CaloSumET','mean',-1,-1,'CaloMETNoHF_BasicCleanup_METTask_CaloSumET_Mean','CaloSumET Mean [GeV]','png'],

  #['MET/CaloMETNoHF/BasicCleanup/METTask_CaloMEx','mean',-1,-1,'CaloMETNoHF_BasicCleanup_METTask_CaloMEx_Mean','CaloMEx Mean [GeV]','png'],

  #['MET/CaloMETNoHF/BasicCleanup/METTask_CaloMEx','rms',-1,-1,'CaloMETNoHF_BasicCleanup_METTask_CaloMEx_RMS','CaloMEx RMS [GeV]','png'],

  #['MET/CaloMETNoHF/BasicCleanup/METTask_CaloMEy','mean',-1,-1,'CaloMETNoHF_BasicCleanup_METTask_CaloMEy_Mean','CaloMEy Mean [GeV]','png'],

  #['MET/CaloMETNoHF/BasicCleanup/METTask_CaloMEy','rms',-1,-1,'CaloMETNoHF_BasicCleanup_METTask_CaloMEy_RMS','CaloMEy RMS [GeV]','png'],

  #['MET/CaloMETNoHF/ExtraCleanup/METTask_CaloMET','mean',-1,-1,'CaloMETNoHF_ExtraCleanup_METTask_CaloMET_Mean','CaloMET Mean [GeV]','png'],

  #['MET/CaloMETNoHF/ExtraCleanup/METTask_CaloSumET','mean',-1,-1,'CaloMETNoHF_ExtraCleanup_METTask_CaloSumET_Mean','CaloSumET Mean [GeV]','png'],

  #['MET/CaloMETNoHF/ExtraCleanup/METTask_CaloMEx','mean',-1,-1,'CaloMETNoHF_ExtraCleanup_METTask_CaloMEx_Mean','CaloMEx Mean [GeV]','png'],

  #['MET/CaloMETNoHF/ExtraCleanup/METTask_CaloMEx','rms',-1,-1,'CaloMETNoHF_ExtraCleanup_METTask_CaloMEx_RMS','CaloMEx RMS [GeV]','png'],

  #['MET/CaloMETNoHF/ExtraCleanup/METTask_CaloMEy','mean',-1,-1,'CaloMETNoHF_ExtraCleanup_METTask_CaloMEy_Mean','CaloMEy Mean [GeV]','png'],

  #['MET/CaloMETNoHF/ExtraCleanup/METTask_CaloMEy','rms',-1,-1,'CaloMETNoHF_ExtraCleanup_METTask_CaloMEy_RMS','CaloMEy RMS [GeV]','png'],
  #### TcMET
  ['MET/TcMET/BasicCleanup/METTask_MET','mean',-1,-1,'TcMET_BasicCleanup_METTask_MET_Mean','MET Mean [GeV]','png'],

  ['MET/TcMET/BasicCleanup/METTask_SumET','mean',-1,-1,'TcMET_BasicCleanup_METTask_SumET_Mean','SumET Mean [GeV]','png'],

  ['MET/TcMET/BasicCleanup/METTask_MEx','mean',-1,-1,'TcMET_BasicCleanup_METTask_MEx_Mean','MEx Mean [GeV]','png'],

 # ['MET/TcMET/BasicCleanup/METTask_MEx','rms',-1,-1,'TcMET_BasicCleanup_METTask_MEx_RMS','MEx RMS [GeV]','png'],

  ['MET/TcMET/BasicCleanup/METTask_MEy','mean',-1,-1,'TcMET_BasicCleanup_METTask_MEy_Mean','MEy Mean [GeV]','png'],

 # ['MET/TcMET/BasicCleanup/METTask_MEy','rms',-1,-1,'TcMET_BasicCleanup_METTask_MEy_RMS','MEy RMS [GeV]','png'],

  ['MET/TcMET/ExtraCleanup/METTask_MET','mean',-1,-1,'TcMET_ExtraCleanup_METTask_MET_Mean','MET Mean [GeV]','png'],

  ['MET/TcMET/ExtraCleanup/METTask_SumET','mean',-1,-1,'TcMET_ExtraCleanup_METTask_SumET_Mean','SumET Mean [GeV]','png'],

  ['MET/TcMET/ExtraCleanup/METTask_MEx','mean',-1,-1,'TcMET_ExtraCleanup_METTask_MEx_Mean','MEx Mean [GeV]','png'],

 # ['MET/TcMET/ExtraCleanup/METTask_MEx','rms',-1,-1,'TcMET_ExtraCleanup_METTask_MEx_RMS','MEx RMS [GeV]','png'],

  ['MET/TcMET/ExtraCleanup/METTask_MEy','mean',-1,-1,'TcMET_ExtraCleanup_METTask_MEy_Mean','MEy Mean [GeV]','png'],

  #['MET/TcMET/ExtraCleanup/METTask_MEy','rms',-1,-1,'TcMET_ExtraCleanup_METTask_MEy_RMS','MEy RMS [GeV]','png'],
  #### MuCorrMET
  #['MET/MuCorrMET/BasicCleanup/METTask_MET','mean',-1,-1,'MuCorrMET_BasicCleanup_METTask_MET_Mean','MET Mean [GeV]','png'],

  #['MET/MuCorrMET/BasicCleanup/METTask_SumET','mean',-1,-1,'MuCorrMET_BasicCleanup_METTask_SumET_Mean','SumET Mean [GeV]','png'],

  #['MET/MuCorrMET/BasicCleanup/METTask_MEx','mean',-1,-1,'MuCorrMET_BasicCleanup_METTask_MEx_Mean','MEx Mean [GeV]','png'],

  #['MET/MuCorrMET/BasicCleanup/METTask_MEx','rms',-1,-1,'MuCorrMET_BasicCleanup_METTask_MEx_RMS','MEx RMS [GeV]','png'],

  #['MET/MuCorrMET/BasicCleanup/METTask_MEy','mean',-1,-1,'MuCorrMET_BasicCleanup_METTask_MEy_Mean','MEy Mean [GeV]','png'],

  #['MET/MuCorrMET/BasicCleanup/METTask_MEy','rms',-1,-1,'MuCorrMET_BasicCleanup_METTask_MEy_RMS','MEy RMS [GeV]','png'],

  #['MET/MuCorrMET/ExtraCleanup/METTask_MET','mean',-1,-1,'MuCorrMET_ExtraCleanup_METTask_MET_Mean','MET Mean [GeV]','png'],

  #['MET/MuCorrMET/ExtraCleanup/METTask_SumET','mean',-1,-1,'MuCorrMET_ExtraCleanup_METTask_SumET_Mean','SumET Mean [GeV]','png'],

  #['MET/MuCorrMET/ExtraCleanup/METTask_MEx','mean',-1,-1,'MuCorrMET_ExtraCleanup_METTask_MEx_Mean','MEx Mean [GeV]','png'],

  #['MET/MuCorrMET/ExtraCleanup/METTask_MEx','rms',-1,-1,'MuCorrMET_ExtraCleanup_METTask_MEx_RMS','MEx RMS [GeV]','png'],

  #['MET/MuCorrMET/ExtraCleanup/METTask_MEy','mean',-1,-1,'MuCorrMET_ExtraCleanup_METTask_MEy_Mean','MEy Mean [GeV]','png'],

  #['MET/MuCorrMET/ExtraCleanup/METTask_MEy','rms',-1,-1,'MuCorrMET_ExtraCleanup_METTask_MEy_RMS','PfMEy RMS [GeV]','png'],
  #### PfMET
  ['MET/PfMET/BasicCleanup/METTask_PfMET','mean',-1,-1,'PfMET_BasicCleanup_METTask_MET_Mean','PfMET Mean [GeV]','png'],

  ['MET/PfMET/BasicCleanup/METTask_PfSumET','mean',-1,-1,'PfMET_BasicCleanup_METTask_SumET_Mean','PfSumET Mean [GeV]','png'],

  ['MET/PfMET/BasicCleanup/METTask_PfMEx','mean',-1,-1,'PfMET_BasicCleanup_METTask_MEx_Mean','PfMEx Mean [GeV]','png'],

 # ['MET/PfMET/BasicCleanup/METTask_PfMEx','rms',-1,-1,'PfMET_BasicCleanup_METTask_MEx_RMS','PfMEx RMS [GeV]','png'],

  ['MET/PfMET/BasicCleanup/METTask_PfMEy','mean',-1,-1,'PfMET_BasicCleanup_METTask_MEy_Mean','PfMEy Mean [GeV]','png'],

 # ['MET/PfMET/BasicCleanup/METTask_PfMEy','rms',-1,-1,'PfMET_BasicCleanup_METTask_MEy_RMS','PfMEy RMS [GeV]','png'],

  ['MET/PfMET/ExtraCleanup/METTask_PfMET','mean',-1,-1,'PfMET_ExtraCleanup_METTask_MET_Mean','PfMET Mean [GeV]','png'],

  ['MET/PfMET/ExtraCleanup/METTask_PfSumET','mean',-1,-1,'PfMET_ExtraCleanup_METTask_SumET_Mean','PfSumET Mean [GeV]','png'],

  ['MET/PfMET/ExtraCleanup/METTask_PfMEx','mean',-1,-1,'PfMET_ExtraCleanup_METTask_MEx_Mean','PfMEx Mean [GeV]','png'],

  #['MET/PfMET/ExtraCleanup/METTask_PfMEx','rms',-1,-1,'PfMET_ExtraCleanup_METTask_MEx_RMS','PfMEx RMS [GeV]','png'],

  ['MET/PfMET/ExtraCleanup/METTask_PfMEy','mean',-1,-1,'PfMET_ExtraCleanup_METTask_MEy_Mean','PfMEy Mean [GeV]','png'],

  #['MET/PfMET/ExtraCleanup/METTask_PfMEy','rms',-1,-1,'PfMET_ExtraCleanup_METTask_MEy_RMS','PfMEy RMS [GeV]','png']
]

#### End of User Section
##################################################################################################################################


if not os.path.isdir(fileLocation):
  print ("Sorry, %s is not a directory"%(fileLocation))
  sys.exit()

x = []
ex = []
fileNames = []

for i in os.listdir(fileLocation):
  if not os.path.isfile(os.path.join(fileLocation,i)):
    continue
  if(i.find("_R")==-1 or not re.search(".root$",i)):
    continue
  run=string.atoi(i[(i.find("_R")+2):(i.find("_R")+11)])
  if(runRangeMode):
    if(len(runRange)<2):
      print "Please define a range of runs to be processed"
      sys.exit()
    if(run>=runRange[0] and run<=runRange[1]):
      if(x.count(run)>0):
        print ("Multiple DQM files found for run %i"%(run))
        print "Aborting"
        sys.exit()
      x.append(run)
      ex.append(0)
      fileNames.append(os.path.join(fileLocation,i))
  else:
    for j in range(len(runNumbers)):
      if(run==runNumbers[j]):
        if(x.count(run)>0):
          print ("Multiple DQM files found for run %i"%(run))
          print "Aborting"
          sys.exit()
        x.append(run)
        ex.append(0)
        fileNames.append(os.path.join(fileLocation,i))


if (len(x)==0):
  print ("No DQM files corresponding to the requested run numbers found in " + fileLocation)
  sys.exit()

y_arrays_transposed = []
ey_arrays_transposed = []

for i in range(len(x)):
  print ("Processing %s"%(fileNames[i]))
  y_tr = []
  ey_tr = []
  try:
    inputFile = TFile(fileNames[i])
    hist = TH1F()
    for j in range(len(dqmMEs)):
      histoName = ("DQMData/Run %i/JetMET/Run summary/"%(x[i])) + dqmMEs[j][0]
      try:
        hist=inputFile.Get(histoName)
        if(dqmMEs[j][1]=="mean"):
          y_tr.append(hist.GetMean())
          ey_tr.append(hist.GetMeanError())
        elif(dqmMEs[j][1]=="rms"):
          y_tr.append(hist.GetRMS())
          ey_tr.append(hist.GetRMSError())
      except:
        print ("Could not find " + histoName + " in " + fileNames[i])
        continue
  except:
    print ("Could not open " + fileNames[i])
    continue
  if(len(y_tr)!=len(dqmMEs) or len(ey_tr)!=len(dqmMEs)):
    print "Could not find some Monitor Elements. Please check that their names were typed in correctly"
    print "Aborting"
    sys.exit()
  y_arrays_transposed.append(y_tr)
  ey_arrays_transposed.append(ey_tr)

if(len(y_arrays_transposed)!=len(x) or len(ey_arrays_transposed)!=len(x)):
  print "Could not open some DQM root files. These files might be corrupted"
  print "Aborting"
  sys.exit()

gROOT.SetBatch(kTRUE);
gStyle.SetCanvasColor(0)
gStyle.SetCanvasBorderMode(0)
gStyle.SetPadColor(0)
gStyle.SetPadBorderMode(0)
gStyle.SetFrameBorderMode(0)
gStyle.SetPadTickX(1)
gStyle.SetPadTickY(1)
gStyle.SetPalette(1)

for i in range(len(dqmMEs)):
  y = []
  ey = []
  for j in range(len(x)):
    y.append(y_arrays_transposed[j][i])
    ey.append(ey_arrays_transposed[j][i])
  c = TCanvas("c");
  c.cd();

  graph = TGraphErrors(len(x),array('d',x),array('d',y),array('d',ex),array('d',ey))
  graph.SetTitle(dqmMEs[i][4])
  graph.GetXaxis().SetTitle("Run Number")
  graph.GetYaxis().SetTitle(dqmMEs[i][5])
  graph.SetMarkerStyle(20)
  graph.SetMarkerColor(kRed)
  if(dqmMEs[i][2]!=dqmMEs[i][3]):
    graph.SetMinimum(dqmMEs[i][2])
  if(dqmMEs[i][2]!=dqmMEs[i][3]):
    graph.SetMaximum(dqmMEs[i][3])
  graph.Draw("ap")

  c.SaveAs((dqmMEs[i][4] + "." + dqmMEs[i][6]))

  del c

print "Output files successfully created"
sys.exit()
