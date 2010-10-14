#!/usr/bin/env python


class dqmME:

    def __init__(self, ME="",extract="mean",ymin=None, ymax=None, title="",ytitle="",format="png"):
        self.ME=ME
        self.extract=extract
        self.ymin=ymin
        self.ymax=ymax
        self.title=title
        self.ytitle=ytitle
        self.format=format

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
    dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloMET',
          title='CaloMET_BasicCleanup_METTask_CaloMET_Mean',
          ytitle='CaloMET Mean [GeV]'),
  dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloSumET',
        title='CaloMET_BasicCleanup_METTask_CaloSumET_Mean',
        ytitle='CaloSumET Mean [GeV]'),

  dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloMEx',
        title='CaloMET_BasicCleanup_METTask_CaloMEx_Mean',
        ytitle='CaloMEx Mean [GeV]'),
 # dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloMEx',extract='rms',title='CaloMET_BasicCleanup_METTask_CaloMEx_RMS',ytitle='CaloMEx RMS [GeV]'),

  dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloMEy',
        title='CaloMET_BasicCleanup_METTask_CaloMEy_Mean',
        ytitle='CaloMEy Mean [GeV]'),
 # dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloMEy',extract='rms',title='CaloMET_BasicCleanup_METTask_CaloMEy_RMS',ytitle='CaloMEy RMS [GeV]'),

  dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloEmEtFraction',
        title='CaloMET_BasicCleanup_METTask_CaloEmEtFraction_Mean',
        ytitle='CaloEmEtFraction Mean '),

  #--- Calo Sum ET from CaloMET block

  dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloEmEtInEB',
        title='CaloMET_BasicCleanup_METTask_CaloEmEtInEB_Mean',
        ytitle='CaloEmEtInEB Mean [GeV]'),
  dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloEmEtInEE',
        title='CaloMET_BasicCleanup_METTask_CaloEmEtInEE_Mean',
        ytitle='CaloEmEtInEE Mean [GeV]'),
  dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloEmEtInHF',
        title='CaloMET_BasicCleanup_METTask_CaloEmEtInHF_Mean',
        ytitle='CaloEmEtInHF Mean [GeV]'),

  dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloHadEtInHB',
        title='CaloMET_BasicCleanup_METTask_CaloHadEtInHB_Mean',
        ytitle='CaloHadEtInHB Mean [GeV]'),
  dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloHadEtInHE',
        title='CaloMET_BasicCleanup_METTask_CaloHadEtInHE_Mean',
        ytitle='CaloHadEtInHE Mean [GeV]'),
  dqmME(ME='MET/CaloMET/BasicCleanup/METTask_CaloHadEtInHF',
        title='CaloMET_BasicCleanup_METTask_CaloHadEtInHF_Mean',
        ytitle='CaloHadEtInHF Mean [GeV]'),

  #--- 

   dqmME(ME='Jet/CleanedAntiKtJets/Pt',
         title='Jet_CleanedAntiKtJets_Pt2_Mean',
         ytitle='Cleaned Jet Pt Mean [GeV]'),
   dqmME(ME='Jet/CleanedAntiKtJets/Pt_Barrel',
         title='Jet_CleanedAntiKtJets_Pt_Barrel_Mean',
         ytitle='Cleaned Jet Pt Barrel Mean [GeV]'),
   dqmME(ME='Jet/CleanedAntiKtJets/Pt_EndCap',
         title='Jet_CleanedAntiKtJets_Pt_Endcap_Mean',
         ytitle='Cleaned Jet Pt Endcap Mean [GeV]'),
   dqmME(ME='Jet/CleanedAntiKtJets/Pt_Forward',
         title='Jet_CleanedAntiKtJets_Pt_Forward_Mean',
         ytitle='Cleaned Jet Pt Forward Mean [GeV]'),
  dqmME(ME='Jet/CleanedAntiKtJets/Constituents',
        title='Jet_CleanedAntiKtJets_Constituents_Mean',
        ytitle='Cleaned Jet Constituents Mean [GeV]'),

##  #---
  
   dqmME(ME='Jet/JPT/Pt',title='Jet_JPT_Pt_Mean',ytitle='Cleaned Jet Pt Mean [GeV]'),
   dqmME(ME='Jet/JPT/Phi',title='Jet_JPT_Phi_Mean',ytitle='Cleaned Jet Phi Mean '),
   dqmME(ME='Jet/JPT/Eta',title='Jet_JPT_Eta_Mean',ytitle='Cleaned Jet Eta  Mean '),
   dqmME(ME='Jet/JPT/nTracks',title='Jet_JPT_nTracks_Mean',ytitle='Cleaned Jet nTracks  Mean '),
  
  
## ----
 dqmME(ME='Jet/PFJets/Pt',title='Jet_PFJets_Pt_Mean',ytitle='Cleaned Jet Pt Mean [GeV]'),
 dqmME(ME='Jet/PFJets/Phi',title='Jet_PFJets_Phi_Mean',ytitle='Cleaned Jet Phi Mean '),
 dqmME(ME='Jet/PFJets/Eta',title='Jet_PFJets_Eta_Mean',ytitle='Cleaned Jet Eta Mean '),
  
  
  
 #  dqmME(ME='Jet/JPT/Constituents',title='Jet_JPT_Constituents_Mean',ytitle='Cleaned Jet Constituents Mean [GeV]'),

  

  #dqmME(ME='MET/CaloMET/ExtraCleanup/METTask_CaloMET',title='CaloMET_ExtraCleanup_METTask_CaloMET_Mean',ytitle='CaloMET Mean [GeV]'),

  #dqmME(ME='MET/CaloMET/ExtraCleanup/METTask_CaloSumET',title='CaloMET_ExtraCleanup_METTask_CaloSumET_Mean',ytitle='CaloSumET Mean [GeV]'),

  #dqmME(ME='MET/CaloMET/ExtraCleanup/METTask_CaloMEx',title='CaloMET_ExtraCleanup_METTask_CaloMEx_Mean',ytitle='CaloMEx Mean [GeV]'),

  #dqmME(ME='MET/CaloMET/ExtraCleanup/METTask_CaloMEx',extract='rms',title='CaloMET_ExtraCleanup_METTask_CaloMEx_RMS',ytitle='CaloMEx RMS [GeV]'),

  #dqmME(ME='MET/CaloMET/ExtraCleanup/METTask_CaloMEy',title='CaloMET_ExtraCleanup_METTask_CaloMEy_Mean',ytitle='CaloMEy Mean [GeV]'),

  #dqmME(ME='MET/CaloMET/ExtraCleanup/METTask_CaloMEy',extract='rms',title='CaloMET_ExtraCleanup_METTask_CaloMEy_RMS',ytitle='CaloMEy RMS [GeV]'),
  #### CaloMETNoHF
  #dqmME(ME='MET/CaloMETNoHF/BasicCleanup/METTask_CaloMET',title='CaloMETNoHF_BasicCleanup_METTask_CaloMET_Mean',ytitle='CaloMET Mean [GeV]'),

  #dqmME(ME='MET/CaloMETNoHF/BasicCleanup/METTask_CaloSumET',title='CaloMETNoHF_BasicCleanup_METTask_CaloSumET_Mean',ytitle='CaloSumET Mean [GeV]'),

  #dqmME(ME='MET/CaloMETNoHF/BasicCleanup/METTask_CaloMEx',title='CaloMETNoHF_BasicCleanup_METTask_CaloMEx_Mean',ytitle='CaloMEx Mean [GeV]'),

  #dqmME(ME='MET/CaloMETNoHF/BasicCleanup/METTask_CaloMEx',extract='rms',title='CaloMETNoHF_BasicCleanup_METTask_CaloMEx_RMS',ytitle='CaloMEx RMS [GeV]'),

  #dqmME(ME='MET/CaloMETNoHF/BasicCleanup/METTask_CaloMEy',title='CaloMETNoHF_BasicCleanup_METTask_CaloMEy_Mean',ytitle='CaloMEy Mean [GeV]'),

  #dqmME(ME='MET/CaloMETNoHF/BasicCleanup/METTask_CaloMEy',extract='rms',title='CaloMETNoHF_BasicCleanup_METTask_CaloMEy_RMS',ytitle='CaloMEy RMS [GeV]'),

  #dqmME(ME='MET/CaloMETNoHF/ExtraCleanup/METTask_CaloMET',title='CaloMETNoHF_ExtraCleanup_METTask_CaloMET_Mean',ytitle='CaloMET Mean [GeV]'),

  #dqmME(ME='MET/CaloMETNoHF/ExtraCleanup/METTask_CaloSumET',title='CaloMETNoHF_ExtraCleanup_METTask_CaloSumET_Mean',ytitle='CaloSumET Mean [GeV]'),

  #dqmME(ME='MET/CaloMETNoHF/ExtraCleanup/METTask_CaloMEx',title='CaloMETNoHF_ExtraCleanup_METTask_CaloMEx_Mean',ytitle='CaloMEx Mean [GeV]'),

  #dqmME(ME='MET/CaloMETNoHF/ExtraCleanup/METTask_CaloMEx',extract='rms',title='CaloMETNoHF_ExtraCleanup_METTask_CaloMEx_RMS',ytitle='CaloMEx RMS [GeV]'),

  #dqmME(ME='MET/CaloMETNoHF/ExtraCleanup/METTask_CaloMEy',title='CaloMETNoHF_ExtraCleanup_METTask_CaloMEy_Mean',ytitle='CaloMEy Mean [GeV]'),

  #dqmME(ME='MET/CaloMETNoHF/ExtraCleanup/METTask_CaloMEy',extract='rms',title='CaloMETNoHF_ExtraCleanup_METTask_CaloMEy_RMS',ytitle='CaloMEy RMS [GeV]'),
  #### TcMET
  dqmME(ME='MET/TcMET/BasicCleanup/METTask_MET',title='TcMET_BasicCleanup_METTask_MET_Mean',ytitle='MET Mean [GeV]'),

  dqmME(ME='MET/TcMET/BasicCleanup/METTask_SumET',title='TcMET_BasicCleanup_METTask_SumET_Mean',ytitle='SumET Mean [GeV]'),

  dqmME(ME='MET/TcMET/BasicCleanup/METTask_MEx',title='TcMET_BasicCleanup_METTask_MEx_Mean',ytitle='MEx Mean [GeV]'),

 # dqmME(ME='MET/TcMET/BasicCleanup/METTask_MEx',extract='rms',title='TcMET_BasicCleanup_METTask_MEx_RMS',ytitle='MEx RMS [GeV]'),

  dqmME(ME='MET/TcMET/BasicCleanup/METTask_MEy',title='TcMET_BasicCleanup_METTask_MEy_Mean',ytitle='MEy Mean [GeV]'),

 # dqmME(ME='MET/TcMET/BasicCleanup/METTask_MEy',extract='rms',title='TcMET_BasicCleanup_METTask_MEy_RMS',ytitle='MEy RMS [GeV]'),

  dqmME(ME='MET/TcMET/ExtraCleanup/METTask_MET',title='TcMET_ExtraCleanup_METTask_MET_Mean',ytitle='MET Mean [GeV]'),

  dqmME(ME='MET/TcMET/ExtraCleanup/METTask_SumET',title='TcMET_ExtraCleanup_METTask_SumET_Mean',ytitle='SumET Mean [GeV]'),

  dqmME(ME='MET/TcMET/ExtraCleanup/METTask_MEx',title='TcMET_ExtraCleanup_METTask_MEx_Mean',ytitle='MEx Mean [GeV]'),

 # dqmME(ME='MET/TcMET/ExtraCleanup/METTask_MEx',extract='rms',title='TcMET_ExtraCleanup_METTask_MEx_RMS',ytitle='MEx RMS [GeV]'),

  dqmME(ME='MET/TcMET/ExtraCleanup/METTask_MEy',title='TcMET_ExtraCleanup_METTask_MEy_Mean',ytitle='MEy Mean [GeV]'),

  #dqmME(ME='MET/TcMET/ExtraCleanup/METTask_MEy',extract='rms',title='TcMET_ExtraCleanup_METTask_MEy_RMS',ytitle='MEy RMS [GeV]'),
  #### MuCorrMET
  #dqmME(ME='MET/MuCorrMET/BasicCleanup/METTask_MET',title='MuCorrMET_BasicCleanup_METTask_MET_Mean',ytitle='MET Mean [GeV]'),

  #dqmME(ME='MET/MuCorrMET/BasicCleanup/METTask_SumET',title='MuCorrMET_BasicCleanup_METTask_SumET_Mean',ytitle='SumET Mean [GeV]'),

  #dqmME(ME='MET/MuCorrMET/BasicCleanup/METTask_MEx',title='MuCorrMET_BasicCleanup_METTask_MEx_Mean',ytitle='MEx Mean [GeV]'),

  #dqmME(ME='MET/MuCorrMET/BasicCleanup/METTask_MEx',extract='rms',title='MuCorrMET_BasicCleanup_METTask_MEx_RMS',ytitle='MEx RMS [GeV]'),

  #dqmME(ME='MET/MuCorrMET/BasicCleanup/METTask_MEy',title='MuCorrMET_BasicCleanup_METTask_MEy_Mean',ytitle='MEy Mean [GeV]'),

  #dqmME(ME='MET/MuCorrMET/BasicCleanup/METTask_MEy',extract='rms',title='MuCorrMET_BasicCleanup_METTask_MEy_RMS',ytitle='MEy RMS [GeV]'),

  #dqmME(ME='MET/MuCorrMET/ExtraCleanup/METTask_MET',title='MuCorrMET_ExtraCleanup_METTask_MET_Mean',ytitle='MET Mean [GeV]'),

  #dqmME(ME='MET/MuCorrMET/ExtraCleanup/METTask_SumET',title='MuCorrMET_ExtraCleanup_METTask_SumET_Mean',ytitle='SumET Mean [GeV]'),

  #dqmME(ME='MET/MuCorrMET/ExtraCleanup/METTask_MEx',title='MuCorrMET_ExtraCleanup_METTask_MEx_Mean',ytitle='MEx Mean [GeV]'),

  #dqmME(ME='MET/MuCorrMET/ExtraCleanup/METTask_MEx',extract='rms',title='MuCorrMET_ExtraCleanup_METTask_MEx_RMS',ytitle='MEx RMS [GeV]'),

  #dqmME(ME='MET/MuCorrMET/ExtraCleanup/METTask_MEy',title='MuCorrMET_ExtraCleanup_METTask_MEy_Mean',ytitle='MEy Mean [GeV]'),

  #dqmME(ME='MET/MuCorrMET/ExtraCleanup/METTask_MEy',extract='rms',title='MuCorrMET_ExtraCleanup_METTask_MEy_RMS',ytitle='PfMEy RMS [GeV]'),
  #### PfMET
  dqmME(ME='MET/PfMET/BasicCleanup/METTask_PfMET',title='PfMET_BasicCleanup_METTask_MET_Mean',ytitle='PfMET Mean [GeV]'),

  dqmME(ME='MET/PfMET/BasicCleanup/METTask_PfSumET',title='PfMET_BasicCleanup_METTask_SumET_Mean',ytitle='PfSumET Mean [GeV]'),

  dqmME(ME='MET/PfMET/BasicCleanup/METTask_PfMEx',title='PfMET_BasicCleanup_METTask_MEx_Mean',ytitle='PfMEx Mean [GeV]'),

 # dqmME(ME='MET/PfMET/BasicCleanup/METTask_PfMEx',extract='rms',title='PfMET_BasicCleanup_METTask_MEx_RMS',ytitle='PfMEx RMS [GeV]'),

  dqmME(ME='MET/PfMET/BasicCleanup/METTask_PfMEy',title='PfMET_BasicCleanup_METTask_MEy_Mean',ytitle='PfMEy Mean [GeV]'),

 # dqmME(ME='MET/PfMET/BasicCleanup/METTask_PfMEy',extract='rms',title='PfMET_BasicCleanup_METTask_MEy_RMS',ytitle='PfMEy RMS [GeV]'),

  dqmME(ME='MET/PfMET/ExtraCleanup/METTask_PfMET',title='PfMET_ExtraCleanup_METTask_MET_Mean',ytitle='PfMET Mean [GeV]'),

  dqmME(ME='MET/PfMET/ExtraCleanup/METTask_PfSumET',title='PfMET_ExtraCleanup_METTask_SumET_Mean',ytitle='PfSumET Mean [GeV]'),

  dqmME(ME='MET/PfMET/ExtraCleanup/METTask_PfMEx',title='PfMET_ExtraCleanup_METTask_MEx_Mean',ytitle='PfMEx Mean [GeV]'),

  #dqmME(ME='MET/PfMET/ExtraCleanup/METTask_PfMEx',extract='rms',title='PfMET_ExtraCleanup_METTask_MEx_RMS',ytitle='PfMEx RMS [GeV]'),

  dqmME(ME='MET/PfMET/ExtraCleanup/METTask_PfMEy',title='PfMET_ExtraCleanup_METTask_MEy_Mean',ytitle='PfMEy Mean [GeV]'),

  #dqmME(ME='MET/PfMET/ExtraCleanup/METTask_PfMEy',extract='rms',title='PfMET_ExtraCleanup_METTask_MEy_RMS',ytitle='PfMEy RMS [GeV]')
]


if __name__=="__main__":

    for i in dqmMEs:
        print i.ME
    print "------------------------------------------"
    print "There are a total of %s MEs "%len(dqmMEs)
