import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi


processName = "HLT";
#REDIGI311X


HLTFilterMuOrEleMC = cms.EDFilter('SingleTopTriggers',
                           HLTriggerResults = cms.InputTag("TriggerResults","",processName),
                           isMC = cms.untracked.bool(False),
                           triggerList = cms.vstring("HLT_Ele22_SW_TighterCaloIdIsol_L1R_v2",
                                                               "HLT_Ele22_SW_TighterCaloIdIsol_L1R_v3",
                                                               "HLT_Ele17_SW_TighterEleIdIsol_L1R_v3",
                                                               "HLT_IsoMu17_v4",
                                                               ),    
                           channel = cms.untracked.int32(1),#Useless now
                           )                         

HLTFilterMuOrEle = cms.EDFilter('SingleTopTriggers',
                           HLTriggerResults = cms.InputTag("TriggerResults","",processName),
                           isMC = cms.untracked.bool(False),
                           triggerList = cms.vstring("HLT_IsoMu17_v5",   
                                                               "HLT_IsoMu17_v6",
                                                               "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1",
                                                               "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2",
                                                               "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3",
                                                               ),    
                
                           channel = cms.untracked.int32(1),#Useless now
                           )                         


HLTFilterMu = cms.EDFilter('SingleTopTriggers',
                           HLTriggerResults = cms.InputTag("TriggerResults","",processName),
                           isMC = cms.untracked.bool(False),
                           triggerList = cms.vstring("HLT_IsoMu17_v5",   
                                                               "HLT_IsoMu17_v6",),
                           channel = cms.untracked.int32(2),#Useless now
                           )                         

HLTFilterEle = cms.EDFilter('SingleTopTriggers',
                            HLTriggerResults = cms.InputTag("TriggerResults","",processName),
                            isMC = cms.untracked.bool(False),
                            triggerList = cms.vstring("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1",
                                                               "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2",
                                                               "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3",
                                                                ),
                            channel = cms.untracked.int32(3),#Useless now
                            )

HLTFilterMuQCD = cms.EDFilter('SingleTopTriggers',
                           HLTriggerResults = cms.InputTag("TriggerResults","",processName),
                           isMC = cms.untracked.bool(False),
                           triggerList = cms.vstring("HLT_Mu17_CentralJet30_v1",
                                                               "HLT_Mu17_CentralJet30_v2",
                                                               "HLT_Mu17_CentralJet30_v3",
                                                               "HLT_Mu17_CentralJet30_v4",
                                                               "HLT_Mu17_CentralJet30_v5",
                                                               "HLT_Mu17_CentralJet30_v6",
                                                               "HLT_Mu17_DiCentralJet30_v1",
                                                               "HLT_Mu17_DiCentralJet30_v2",
                                                               "HLT_Mu17_DiCentralJet30_v3",
                                                               "HLT_Mu17_DiCentralJet30_v4",
                                                               "HLT_Mu17_DiCentralJet30_v5",
                                                               "HLT_Mu17_DiCentralJet30_v6",
                                                               ),
                           channel = cms.untracked.int32(4),#Useless now
                           )                         

HLTFilterEleQCD = cms.EDFilter('SingleTopTriggers',
                            HLTriggerResults = cms.InputTag("TriggerResults","",processName),
                            isMC = cms.untracked.bool(False),
                               triggerList = cms.vstring("HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30_v2",
                                                                   "HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30_v3",
                                                                   "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet40_BTagIP",
                                                                   ),
                               channel = cms.untracked.int32(5),#Useless now
                            )

