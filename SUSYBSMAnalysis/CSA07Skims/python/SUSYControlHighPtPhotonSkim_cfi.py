import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.SUSYHighPtPhotonHLTPaths_cfi import *
from SUSYBSMAnalysis.CSA07Skims.SUSYHighPtPhotonCandidates_cfi import *
SUSYControlHighPtPhotonFilter = cms.EDFilter("SUSYControlHighPtPhotonSkim",
    PhotonPtmin = cms.double(20.0),
    Electronsrc = cms.InputTag("pixelMatchGsfElectrons"),
    IsIsolated = cms.bool(False),
    IsolationCut = cms.double(9.0),
    Photonsrc = cms.InputTag("allPhotonIsolations"),
    ElectronPtmin = cms.double(80.0)
)

SUSYControlHighPtPhotonSeq = cms.Sequence(SUSYControlHighPtPhotonHLTPath+allTracks+allPhotons+allPhotonIsolations+SUSYControlHighPtPhotonFilter)

