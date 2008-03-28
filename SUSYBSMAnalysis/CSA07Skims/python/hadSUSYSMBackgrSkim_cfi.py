import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.hadSUSYHLTPaths_cfi import *
hadSUSYSMBackgrFilter = cms.EDFilter("HadSUSYSMBackgrSkim",
    CaloMETsrc = cms.InputTag("met"),
    CaloJetsrc = cms.InputTag("iterativeCone5CaloJets"),
    CaloMETmin = cms.double(100.0),
    NminCaloJet = cms.int32(2),
    CaloJet1Ptmin = cms.double(110.0),
    CaloJetPtmin = cms.double(30.0)
)

hadSUSYSMBackgrSeq = cms.Sequence(hadSUSYHLTJetMETPath+hadSUSYSMBackgrFilter)

