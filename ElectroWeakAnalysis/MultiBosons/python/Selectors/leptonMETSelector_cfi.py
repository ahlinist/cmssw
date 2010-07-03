import FWCore.ParameterSet.Config as cms
import ElectroWeakAnalysis.MultiBosons.Selectors.muonSelector_cfi as muonSelections
import ElectroWeakAnalysis.MultiBosons.Selectors.electronSelector_cfi as electronSelections
import ElectroWeakAnalysis.MultiBosons.Selectors.metSelector_cfi as metSelections

leptonMET_base_selection = cms.PSet(trigSrc = cms.InputTag("patTriggerEvent"),
                                    muonSrc = cms.InputTag("cleanPatMuonsTriggerMatch"),
                                    electronSrc = cms.InputTag("cleanPatElectronsTriggerMatch"),
                                    muonPlusMETSrc = cms.InputTag("muonPlusMETs"),
                                    electronPlusMETSrc = cms.InputTag("electronPlusMETs"),
                                    muTrig = cms.string("HLT_Mu9"),
                                    eleTrig = cms.string(""),
                                    minMT = cms.double(50.0),
                                    minAcoplanarity = cms.double(2.0),
                                    muonId = muonSelections.Jun262010_jpsi_selection.copy(),
                                    electronId = electronSelections.Jul012010_selection.copy(),
                                    metId = metSelections.Jul022010_selection.copy()
                                    )
                                   
