import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.hadSUSYHLTPaths_cfi import *
hadSUSYQCDControlMETFilter = cms.EDFilter("HadSUSYQCDControlMETSkim",
    CaloJetsrc = cms.InputTag("iterativeCone5CaloJets"),
    NminCaloJet = cms.int32(2),
    CaloJetPtmin = cms.double(100.0)
)

hadSUSYQCDControlMETSeq = cms.Sequence(hadSUSYHLTQCDPath+hadSUSYQCDControlMETFilter)

