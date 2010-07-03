import FWCore.ParameterSet.Config as cms

Jul012010_selection = cms.PSet(version = cms.string("Jul012010"),                               
                               ElectronID = cms.string("simpleEleId90relIso"),
                               ElectronIDMinRank = cms.int32(7),                               
                               minPt = cms.double(20),
                               maxEta = cms.double(2.5)
                               )

