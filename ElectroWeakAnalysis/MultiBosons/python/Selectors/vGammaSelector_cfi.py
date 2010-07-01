import FWCore.ParameterSet.Config as cms
import ElectroWeakAnalysis.MultiBosons.Selectors.diLeptonSelector_cfi as diLeptonSelections

sw_commissioning_selection = cms.PSet(trigSrc = cms.InputTag("TriggerResults"),
                                      photonSrc = cms.InputTag("selectedLayer1Photons"),
                                      zeegSrc = cms.InputTag("ZEEGammaCandidates"),
                                      zmumugSrc = cms.InputTag("ZMuMuGammaCandidates"),
                                      wenugSrc = cms.InputTag("WENuGammaCandidates"),
                                      wmunuSrc = cms.InputTag("WMuNuGammaCandidates"),
                                      photonMETSrc = cms.InputTag("InvisibleZGammaCandidates"),
                                      muTrig = cms.string("*Mu*"),                                  
                                      eleTrig = cms.string(""),
                                      photonTrig = cms.string(""),
                                      metTrig = cms.string(""),
                                      diLeptonId = diLeptonSelections.diLepton_jpsi_selection.copy()
                                      )
sw_commissioning_selection.diLeptonId.cutsToIgnore = cms.vstring("Trigger")
