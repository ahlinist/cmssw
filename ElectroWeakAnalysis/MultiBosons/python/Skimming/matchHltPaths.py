"""
The matchHltPaths dictionary specifies the HLT trigger match target and
match collections.  The list of target paths is also used in the
HLT filter of the different skims:
  - cleanPatElectrons: ElectronPhoton skim
  - cleanPatMuons: MuonPhoton skim, Dimuon skim
  - cleanPatJets: Jet skim
"""

metTriggerMatchPaths =     """
                           """.split()

matchHltPaths = {
    "cleanPatElectrons":    """
                            HLT_Photon10_L1R
                            HLT_Photon15_Cleaned_L1R
                            HLT_Ele15_LW_L1R
                            HLT_Ele15_SW_L1R
                            HLT_Ele15_SW_CaloEleId_L1R
                            HLT_Ele17_SW_CaloEleId_L1R
                            HLT_Ele17_SW_TightEleId_L1R
                            HLT_Ele17_SW_TighterEleIdIsol_L1R_v2
                            HLT_Ele17_SW_TighterEleIdIsol_L1R_v3
                            HLT_DoubleEle10_SW_L1R
                            HLT_Ele17_SW_TightCaloEleId_SC8HE_L1R_v1
                            HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1
                            HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2
                            HLT_DoubleEle15_SW_L1R_v1
                            HLT_DoubleEle17_SW_L1R_v1
                            """.split(),

    "cleanPatMuons"   :     """
                            HLT_Mu9
                            HLT_Mu11
                            HLT_Mu15_v1
                            """.split(),

    "cleanPatTaus"    :     """
                            """.split(),

    "cleanPatPhotons" :     """
                            HLT_Ele10_LW_L1R
                            HLT_Ele12_SW_EleIdIsol_L1R
                            HLT_Ele15_LW_L1R
                            HLT_Ele15_SW_EleId_L1R
                            HLT_Ele15_SW_L1R
                            HLT_Ele15_SW_LooseTrackIso_L1R
                            HLT_Ele17_SW_CaloEleId_L1R
                            HLT_Ele17_SW_EleIdIsol_L1R
                            HLT_Ele17_SW_LEleId_L1R
                            HLT_Ele17_SW_TightEleId_L1R
                            HLT_Ele20_SW_L1R
                            HLT_Photon10_L1R
                            HLT_Photon15_L1R
                            HLT_Photon10_Cleaned_L1R
                            HLT_Photon15_Cleaned_L1R
                            HLT_Photon20_Cleaned_L1R
                            HLT_Photon25_Cleaned_L1R
                            HLT_Photon30_Cleaned_L1R
                            HLT_Photon50_Cleaned_L1R
                            HLT_Photon70_Cleaned_L1R
                            HLT_DoublePhoton17_L1R
                            """.split(),

    "cleanPatJets"     :    """
                            HLT_Jet15U
                            HLT_Jet15U_v2
                            HLT_Jet15U_v3
                            HLT_Jet30U
                            HLT_Jet30U_v2
                            HLT_Jet30U_v3
                            HLT_Jet50U
                            HLT_Jet50U_v2
                            HLT_Jet50U_v3
                            HLT_Jet70U
                            HLT_Jet70U_v2
                            HLT_Jet70U_v3
                            HLT_Jet100U
                            HLT_Jet100U_v2
                            HLT_Jet100U_v3
                            HLT_Jet140U_v1
                            HLT_Jet140U_v2
                            HLT_Jet140U_v3
                            """.split(),

    "patMETs"          :    metTriggerMatchPaths,
    "patMETsPF"        :    metTriggerMatchPaths,
    "patMETsTC"        :    metTriggerMatchPaths
  }
