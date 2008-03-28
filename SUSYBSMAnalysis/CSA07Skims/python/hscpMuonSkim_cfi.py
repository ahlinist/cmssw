import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.hscpHLTPaths_cfi import *
#include "RecoTracker/TrackProducer/data/RefitterWithMaterial.cff"
#replace TrackRefitter.TrajectoryInEvent = true
from RecoTracker.DeDx.dedxHitsFromRefitter_cfi import *
highPtMuons = cms.EDFilter("PtMinMuonCountFilter",
    src = cms.InputTag("muons"),
    ptMin = cms.double(45.0),
    minNumber = cms.uint32(1)
)

hscpMuonSeq = cms.Sequence(hscpMuonHLTPath+cms.SequencePlaceholder("TrackRefitter")+dedxHitsFromRefitter+highPtMuons)

