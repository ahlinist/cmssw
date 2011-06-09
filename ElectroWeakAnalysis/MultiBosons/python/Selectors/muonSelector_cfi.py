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
                               maxIso = cms.double(0.15) #for this cut version this is Relative Combined Isolation
                               )

Sep062010_selection = cms.PSet(version = cms.string("Sep062010"),
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
                               maxIso = cms.double(3.0) # here the isolation is tracker absolute isolation
                               )

Jun262010_jpsi_selection = cms.PSet(version = cms.string("Jun262010_jpsi"),
                                    muonTypeRequirement = cms.vstring("AllTrackerMuons"),
                                    TrackerMuonIdType = cms.string("TMLastStationAngTight"),
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

muonSelection_FsrApr082011 = cms.PSet(
    version = cms.string("FsrApr082011"),
    isGlobalMuon = cms.bool(True),
    maxNormChi2 = cms.double(10),
    minMuonHits = cms.uint32(1),
    isTrackerMuon = cms.bool(True),
    minMatches = cms.uint32(2),
    minTkHits = cms.uint32(11),
    minPixelHits = cms.uint32(1),
    maxAbsTrackDxyBS = cms.double(0.2),
    maxTrackIso = cms.double(3),
    minPt = cms.double(10),
    maxAbsEta = cms.double(2.4),
)

muonSelection_FsrApr082011_PixelMatchVeto = cms.PSet(
    version = cms.string("FsrApr082011"),
    isGlobalMuon = cms.bool(True),
    maxNormChi2 = cms.double(10),
    minMuonHits = cms.uint32(1),
    isTrackerMuon = cms.bool(True),
    minMatches = cms.uint32(2),
    minTkHits = cms.uint32(11),
    minPixelHits = cms.uint32(1),
    maxAbsTrackDxyBS = cms.double(0.2),
    maxTrackIso = cms.double(3),
    minPt = cms.double(10),
    maxAbsEta = cms.double(2.4),
    cutsToIgnore = cms.vstring("maxTrackIso"),
)

