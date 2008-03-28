import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.hadSUSYHLTPaths_cfi import *
hadSUSYdiMuonFilter = cms.EDFilter("HadSUSYdiMuonSkim",
    PtmindiMuon = cms.double(150.0),
    MuonPtmin = cms.double(30.0),
    Muonsrc = cms.InputTag("muons"),
    NminMuon = cms.int32(2)
)

hadSUSYdiMuonSeq = cms.Sequence(hadSUSYHLTdiMuonPath+hadSUSYdiMuonFilter)

