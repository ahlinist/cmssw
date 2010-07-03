import FWCore.ParameterSet.Config as cms
import ElectroWeakAnalysis.MultiBosons.Selectors.muonSelector_cfi as muonSelections
import ElectroWeakAnalysis.MultiBosons.Selectors.electronSelector_cfi as electronSelections

diLepton_jpsi_selection = cms.PSet(trigSrc = cms.InputTag("patTriggerEvent"),
                                   muonSrc = cms.InputTag("cleanPatMuonsTriggerMatch"),
                                   electronSrc = cms.InputTag("cleanPatElectronsTriggerMatch"),
                                   dimuonSrc = cms.InputTag("dimuons"),
                                   dielectronSrc = cms.InputTag("dielectrons"),
                                   muTrig = cms.string("HLT_Mu9"),
                                   eleTrig = cms.string(""),
                                   minMass = cms.double(0.0),
                                   muonId1 = muonSelections.Jun262010_jpsi_selection.copy(),
                                   muonId2 = muonSelections.Jun262010_jpsi_selection.copy(),
                                   electronId1 = electronSelections.Jul012010_selection.copy(),
                                   electronId2 = electronSelections.Jul012010_selection.copy()
                                   )

diLepton_zgamma_selection = cms.PSet(trigSrc = cms.InputTag("patTriggerEvent"),
                                     muonSrc = cms.InputTag("cleanPatMuonsTriggerMatch"),
                                     electronSrc = cms.InputTag("cleanPatElectronsTriggerMatch"),
                                     dimuonSrc = cms.InputTag("dimuons"),
                                     dielectronSrc = cms.InputTag("dielectrons"),
                                     muTrig = cms.string("HLT_Mu9"),
                                     eleTrig = cms.string(""),
                                     minMass = cms.double(20.0),
                                     muonId1 = muonSelections.Jun252010_selection.copy(),
                                     muonId2 = muonSelections.Jun252010_selection.copy(),
                                     electronId1 = electronSelections.Jul012010_selection.copy(),
                                     electronId2 = electronSelections.Jul012010_selection.copy()
                                     )

