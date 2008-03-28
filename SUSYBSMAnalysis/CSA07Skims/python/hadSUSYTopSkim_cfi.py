import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.hadSUSYHLTPaths_cfi import *
# Muon + jets 
hadSUSYTopMuonFilter = cms.EDFilter("HadSUSYTopSkim",
    Elecsrc = cms.InputTag("pixelMatchGsfElectrons"),
    MuonPtmin = cms.double(20.0),
    CaloJetsrc = cms.InputTag("iterativeCone5CaloJets"),
    NminElec = cms.int32(0),
    ElecPtmin = cms.double(20.0),
    NminCaloJet = cms.int32(3),
    Muonsrc = cms.InputTag("muons"),
    NminMuon = cms.int32(1),
    CaloJetPtmin = cms.double(30.0)
)

# Electron + jets
hadSUSYTopElecFilter = cms.EDFilter("HadSUSYTopSkim",
    Elecsrc = cms.InputTag("pixelMatchGsfElectrons"),
    MuonPtmin = cms.double(20.0),
    CaloJetsrc = cms.InputTag("iterativeCone5CaloJets"),
    NminElec = cms.int32(1),
    ElecPtmin = cms.double(20.0),
    NminCaloJet = cms.int32(3),
    Muonsrc = cms.InputTag("muons"),
    NminMuon = cms.int32(0),
    CaloJetPtmin = cms.double(30.0)
)

hadSUSYTopMuonSeq = cms.Sequence(hadSUSYHLTTopMuonPath+hadSUSYTopMuonFilter)
hadSUSYTopElecSeq = cms.Sequence(hadSUSYHLTTopElecPath+hadSUSYTopElecFilter)

