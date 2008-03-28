import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.tevDiMuonTrigger_cfi import *
tevDiMuonHighPtFilter = cms.EDFilter("TeVdiMuonSkim",
    MuonPtmin = cms.double(200.0),
    Muonsrc = cms.InputTag("globalMuons")
)

tevDiMuonHighPtSeq = cms.Sequence(tevDiMuonTrigger+tevDiMuonHighPtFilter)

