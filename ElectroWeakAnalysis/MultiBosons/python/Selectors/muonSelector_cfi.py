import FWCore.ParameterSet.Config as cms

Jun252010_selection = cms.PSet(version = cms.string("Jun252010"),
                               muonTypeRequirement = cms.vstring("AllGlobalMuons",
                                                                 "AllTrackerMuons"),
                               TrackerMuonIdType = cms.string("TMLastStationTight"),
                               minPixHits = cms.uint32(0),
                               minTkHits = cms.uint32(10),
                               maxD0 = cms.double(.2),
                               maxDZ = cms.double(15), # not used in this selection!!
                               maxChi2 = cms.double(10),
                               minChambers = cms.uint32(0),
                               minStations = cms.uint32(1),
                               minP = cms.double(0), # not used in this selection
                               minPt = cms.double(20),
                               maxEta = cms.double(2.4),
                               maxRelIso = cms.double(0.15)
                               )

Jun262010_jpsi_selection = cms.PSet(version = cms.string("Jun262010_jpsi"),
                                    muonTypeRequirement = cms.vstring("AllTrackerMuons"),
                                    TrackerMuonIdType = cms.string("TMLastStationAngTightCleaned"),
                                    minPixHits = cms.uint32(0),  # not used in this selection!!
                                    minTkHits = cms.uint32(12),
                                    maxD0 = cms.double(.5),
                                    maxDZ = cms.double(5),
                                    maxChi2 = cms.double(5),
                                    minChambers = cms.uint32(0),  # not used in this selection!!
                                    minStations = cms.uint32(1),  # not used in this selection!!
                                    minP = cms.double(2), 
                                    minPt = cms.double(0),
                                    maxEta = cms.double(2.4),
                                    maxRelIso = cms.double(0.15)
                                    )
