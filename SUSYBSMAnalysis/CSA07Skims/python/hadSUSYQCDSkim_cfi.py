import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.hadSUSYHLTPaths_cfi import *
hadSUSYQCDFilter = cms.EDFilter("HadSUSYQCDSkim",
    CaloJetsrc = cms.InputTag("iterativeCone5CaloJets"),
    NminCaloJet = cms.int32(2),
    CaloJetPtmin = cms.double(100.0),
    CaloMETsrc = cms.InputTag("met")
)

hadSUSYQCDSeq = cms.Sequence(hadSUSYHLTQCDPath+hadSUSYQCDFilter)

