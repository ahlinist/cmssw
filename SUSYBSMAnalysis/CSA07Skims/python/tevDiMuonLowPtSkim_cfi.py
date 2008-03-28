import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.tevDiMuonTrigger_cfi import *
tevDiMuonLowPtFilter = cms.EDFilter("TeVdiMuonSkim",
    MuonPtmin = cms.double(100.0),
    Muonsrc = cms.InputTag("globalMuons")
)

tevDiMuonLowPtSeq = cms.Sequence(tevDiMuonTrigger+tevDiMuonLowPtFilter)

