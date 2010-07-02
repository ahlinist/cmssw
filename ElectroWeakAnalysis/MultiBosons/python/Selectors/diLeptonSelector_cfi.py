import FWCore.ParameterSet.Config as cms
import ElectroWeakAnalysis.MultiBosons.Selectors.muonSelector_cfi as muonSelections
import ElectroWeakAnalysis.MultiBosons.Selectors.electronSelector_cfi as electronSelections

diLepton_jpsi_selection = cms.PSet(trigSrc = cms.InputTag("TriggerResults"),
                                   muonSrc = cms.InputTag("selectedLayer1Muons"),
                                   electronSrc = cms.InputTag("selectedLayer1Electrons"),
                                   diMuonSrc = cms.InputTag("dimuons"),
                                   diElectronSrc = cms.InputTag("dielectrons"),
                                   muTrig = cms.string("*Mu*"),
                                   eleTrig = cms.string(""),
                                   muonId = muonSelections.Jun262010_jpsi_selection.copy(),
                                   electronId = electronSelections.Jul012010_selection.copy()
                                   )
                                   
