import FWCore.ParameterSet.Config as cms
import ElectroWeakAnalysis.MultiBosons.Selectors.diLeptonSelector_cfi as diLeptonSelections
import ElectroWeakAnalysis.MultiBosons.Selectors.leptonMETSelector_cfi as leptonMETSelections
import ElectroWeakAnalysis.MultiBosons.Selectors.photonSelector_cfi as photonSelections

sw_commissioning_selection = cms.PSet(trigSrc = cms.InputTag("patTriggerEvent"),
                                      muonSrc = cms.InputTag("cleanPatMuonsTriggerMatch"),
                                      electronSrc = cms.InputTag("cleanPatElectronsTriggerMatch"),
                                      photonSrc = cms.InputTag("cleanPatPhotonsTriggerMatch"),
                                      metSrc = cms.InputTag("patMETsTriggerMatch"),
                                      dimuonSrc = cms.InputTag("dimuons"),
                                      dielectronSrc = cms.InputTag("dielectrons"),
                                      electronMETSrc = cms.InputTag("electronPlusMETs"),
                                      muonMETSrc = cms.InputTag("muonPlusMETs"),
                                      zeegSrc = cms.InputTag("ZEEGammaCands"),
                                      zmumugSrc = cms.InputTag("ZMuMuGammaCands"),
                                      wenugSrc = cms.InputTag("WENuGammaCands"),
                                      wmunugSrc = cms.InputTag("WMuNuGammaCands"),
                                      znunugSrc = cms.InputTag("ZInvisibleGammaCands"),
                                      muTrig = cms.string("HLT_Mu9"),
                                      eleTrig = cms.string(""),
                                      photonTrig = cms.string(""),
                                      metTrig = cms.string(""),
                                      diLeptonIdCfg = diLeptonSelections.diLepton_jpsi_selection.copy(),
                                      leptonPlusMETIdCfg = leptonMETSelections.leptonMET_base_selection.copy(),
                                      zGammaPhotonIdCfg = photonSelections.Jul022010_loose_selection.copy(),
                                      wGammaPhotonIdCfg = photonSelections.Jul022010_poter65_selection.copy()
                                      )
sw_commissioning_selection.diLeptonIdCfg.cutsToIgnore = cms.vstring("Trigger")
