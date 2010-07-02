import FWCore.ParameterSet.Config as cms
import ElectroWeakAnalysis.MultiBosons.Selectors.diLeptonSelector_cfi as diLeptonSelections
import ElectroWeakAnalysis.MultiBosons.Selectors.leptonMETSelector_cfi as leptonMETSelections
import ElectroWeakAnalysis.MultiBosons.Selectors.photonSelector_cfi as photonSelections

sw_commissioning_selection = cms.PSet(trigSrc = cms.InputTag("TriggerResults"),
                                      muonSrc = cms.InputTag("selectedLayer1Muons"),
                                      electronSrc = cms.InputTag("selectedLayer1Electrons"),
                                      photonSrc = cms.InputTag("selectedLayer1Photons"),
                                      metSrc = cms.InputTag("selectedLayer1METs"),
                                      dimuonSrc = cms.InputTag("dimuons"),
                                      dielectronSrc = cms.InputTag("dielectrons"),
                                      electronMETSrc = cms.InputTag("electronMETs"),
                                      muonMETSrc = cms.InputTag("muonMETs"),
                                      zeegSrc = cms.InputTag("ZEEGammaCands"),
                                      zmumugSrc = cms.InputTag("ZMuMuGammaCands"),
                                      wenugSrc = cms.InputTag("WENuGammaCands"),
                                      wmunuSrc = cms.InputTag("WMuNuGammaCands"),
                                      znunugSrc = cms.InputTag("ZNuNuGammaCands"),
                                      muTrig = cms.string("*Mu*"),
                                      eleTrig = cms.string(""),
                                      photonTrig = cms.string(""),
                                      metTrig = cms.string(""),
                                      diLeptonId = diLeptonSelections.diLepton_jpsi_selection.copy(),
                                      leptonMETId = leptonMETSelections.leptonMET_base_selection.copy(),
                                      zGammaPhotonId = photonSelections.Jul022010_loose_selection.copy(),
                                      wGammaPhotonId = photonSelections.Jul022010_poter65_selection.copy()
                                      )
sw_commissioning_selection.diLeptonId.cutsToIgnore = cms.vstring("Trigger")
