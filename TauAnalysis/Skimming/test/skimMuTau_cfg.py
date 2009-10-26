import FWCore.ParameterSet.Config as cms

process = cms.Process("muTauSkim")

from TauAnalysis.Skimming.EventContent_cff import *

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('MC_31X_V2::All')
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_3_1_2/RelValZTT/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/A4DD1FAE-B178-DE11-B608-001D09F24EAC.root',
        '/store/relval/CMSSW_3_1_2/RelValZTT/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/9408B54D-CB78-DE11-9AEB-001D09F2503C.root'
    )
)

#--------------------------------------------------------------------------------
# select muons and tau-jets
#--------------------------------------------------------------------------------

process.selectedMuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag('muons'),
    cut = cms.string("pt > 8 & abs(eta) < 2.5"),
    filter = cms.bool(True)
)

process.selectedPFTaus = cms.EDFilter("PFTauSelector",
    src = cms.InputTag('shrinkingConePFTauProducer'),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingPionPtCut"),
            selectionCut = cms.double(0.5)
        )
    ),
    filter = cms.bool(True)
)

process.selectedCaloTaus = cms.EDFilter("CaloTauSelector",
    src = cms.InputTag('caloRecoTauProducer'),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator = cms.InputTag("caloRecoTauDiscriminationByLeadingTrackPtCut"),
            selectionCut = cms.double(0.5)
        )
    ),
    filter = cms.bool(True)
)

#--------------------------------------------------------------------------------
# combine selected muons and tau-jets into pairs;
# require muons and tau-jets to be separated by dR > 0.3 in eta-phi,
# in order to reject events in which the tau-jet refers to the same physical
# particle as the muon (note that almost all muons get selected as tau-jets !!)
#--------------------------------------------------------------------------------

process.muCaloTauPairs = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedCaloTaus'),
    srcLeg2 = cms.InputTag('selectedMuons'),
    dRmin12 = cms.double(0.),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)                                       
)

process.selectedMuCaloTauPairs = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('muCaloTauPairs'),
    cut = cms.string("dR12 > 0.3"),
    filter = cms.bool(True)                                     
)

process.muPFTauPairs = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPFTaus'),
    srcLeg2 = cms.InputTag('selectedMuons'),
    dRmin12 = cms.double(0.),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

process.selectedMuPFTauPairs = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('muPFTauPairs'),
    cut = cms.string("dR12 > 0.3"),
    filter = cms.bool(True)                                     
)

#--------------------------------------------------------------------------------
# keep event in case it passed either the muon + pfTau or muon + caloTau selection
#--------------------------------------------------------------------------------

process.muPFTauSkimPath = cms.Path(
    (process.selectedPFTaus + process.selectedMuons)
   * process.muPFTauPairs
   * process.selectedMuPFTauPairs
)

process.muCaloTauSkimPath = cms.Path(
    (process.selectedCaloTaus + process.selectedMuons)
   * process.muCaloTauPairs
   * process.selectedMuCaloTauPairs
)

muTauEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('muPFTauSkimPath', 'muCaloTauSkimPath')
    )
)

process.muTauSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    tauAnalysisEventContent,                                               
    muTauEventSelection,
    fileName = cms.untracked.string('muTauSkim.root')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath( process.muTauSkimOutputModule )

