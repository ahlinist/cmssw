import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.SUSYHighPtPhotonHLTPaths_cfi import *
from SUSYBSMAnalysis.CSA07Skims.SUSYHighPtPhotonCandidates_cfi import *
SUSYHighPtPhotonFilter = cms.EDFilter("SUSYHighPtPhotonSkim",
    Photon2Ptmin = cms.double(20.0),
    IsIsolated = cms.bool(False),
    IsolationCut = cms.double(9.0),
    Photon1Ptmin = cms.double(80.0),
    Photonsrc = cms.InputTag("allPhotonIsolations")
)

SUSYHighPtPhotonSeq = cms.Sequence(SUSYHighPtPhotonHLTPath+allTracks+allPhotons+allPhotonIsolations+SUSYHighPtPhotonFilter)

