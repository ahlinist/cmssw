import FWCore.ParameterSet.Config as cms
import ElectroWeakAnalysis.MultiBosons.Selectors.diLeptonSelector_cfi as diLeptonSelections
import ElectroWeakAnalysis.MultiBosons.Selectors.leptonMETSelector_cfi as leptonMETSelections
import ElectroWeakAnalysis.MultiBosons.Selectors.photonSelector_cfi as photonSelections

sw_commissioning_selection = cms.PSet(verbose_output = cms.bool(False),
                                      trigSrc = cms.InputTag("patTriggerEvent","","PAT"),
                                      muonSrc = cms.InputTag("cleanPatMuonsTriggerMatch","","PAT"),
                                      electronSrc = cms.InputTag("cleanPatElectronsTriggerMatch","","PAT"),
                                      photonSrc = cms.InputTag("cleanPatPhotonsTriggerMatch","","PAT"),
                                      metSrc = cms.InputTag("patMETsTriggerMatch","","PAT"),
                                      dimuonSrc = cms.InputTag("dimuons","","PAT"),
                                      dielectronSrc = cms.InputTag("dielectrons","","PAT"),
                                      electronMETSrc = cms.InputTag("electronPlusMETs","","PAT"),
                                      muonMETSrc = cms.InputTag("muonPlusMETs","","PAT"),
                                      zeegSrc = cms.InputTag("ZEEGammaCands","","PAT"),
                                      zmumugSrc = cms.InputTag("ZMuMuGammaCands","","PAT"),
                                      wenugSrc = cms.InputTag("WENuGammaCands","","PAT"),
                                      wmunugSrc = cms.InputTag("WMuNuGammaCands","","PAT"),
                                      znunugSrc = cms.InputTag("ZInvisibleGammaCands","","PAT"),
                                      muTrig = cms.string("HLT_Mu9"),
                                      eleTrig = cms.string("HLT_Ele15_LW_L1R"),
                                      photonTrig = cms.string(""),
                                      metTrig = cms.string(""),
                                      minLeptonPhotonDeltaR = cms.double(0.7),
                                      diLeptonIdCfg = diLeptonSelections.diLepton_zgamma_selection.copy(),
                                      leptonPlusMETIdCfg = leptonMETSelections.leptonMET_base_selection.copy(),
                                      zGammaPhotonIdCfg = photonSelections.Jul022010_loose_selection.copy(),
                                      wGammaPhotonIdCfg = photonSelections.Jul022010_poter65_selection.copy()
                                      )
sw_commissioning_selection.diLeptonIdCfg.cutsToIgnore = cms.vstring("Trigger")
