import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process("diTauSkim")

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
# select tau-jets 
#--------------------------------------------------------------------------------

process.selectedPFTausLeadPionPt8 = cms.EDFilter("PFTauSelector",
    src = cms.InputTag('shrinkingConePFTauProducer'),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingPionPtCut"),
            selectionCut = cms.double(0.5)
        )
    ),
    cut = cms.string("abs(eta) < 2.5 & leadPFCand().isNonnull() & leadPFCand().pt() > 8."),
    filter = cms.bool(True)
)

process.selectedPFTausLeadPionPt12 = copy.deepcopy(process.selectedPFTausLeadPionPt8)
process.selectedPFTausLeadPionPt12.cut = cms.string("abs(eta) < 2.5 & leadPFCand().isNonnull() & leadPFCand().pt() > 12.")

process.selectedCaloTausLeadTrackPt8 = cms.EDFilter("CaloTauSelector",
    src = cms.InputTag('caloRecoTauProducer'),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator = cms.InputTag("caloRecoTauDiscriminationByLeadingTrackPtCut"),
            selectionCut = cms.double(0.5)
        )
    ),
    cut = cms.string("abs(eta) < 2.5 & leadTrack().isNonnull() & leadTrack().pt() > 8."),                                    
    filter = cms.bool(True)
)

process.selectedCaloTausLeadTrackPt12 = copy.deepcopy(process.selectedCaloTausLeadTrackPt8)
process.selectedCaloTausLeadTrackPt12.cut = cms.string("abs(eta) < 2.5 & leadTrack().isNonnull() & leadTrack().pt() > 12.")

#--------------------------------------------------------------------------------
# combine selected tau-jets into pairs;
# require the two tau-jets to be separated by dR > 0.3 in eta-phi
#--------------------------------------------------------------------------------

process.caloTauPairs = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedCaloTausLeadTrackPt8'),
    srcLeg2 = cms.InputTag('selectedCaloTausLeadTrackPt12'),
    dRmin12 = cms.double(0.),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)                                       
)

process.selectedCaloTauPairs = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('caloTauPairs'),
    cut = cms.string("dR12 > 0.3"),
    filter = cms.bool(True)                                     
)

process.pfTauPairs = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPFTausLeadPionPt8'),
    srcLeg2 = cms.InputTag('selectedPFTausLeadPionPt12'),
    dRmin12 = cms.double(0.),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

process.selectedPFTauPairs = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('pfTauPairs'),
    cut = cms.string("dR12 > 0.3"),
    filter = cms.bool(True)                                     
)

process.mixedTauPairsI = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedCaloTausLeadTrackPt8'),
    srcLeg2 = cms.InputTag('selectedPFTausLeadPionPt12'),
    dRmin12 = cms.double(0.),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

process.selectedMixedTauPairsI = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('mixedTauPairsI'),
    cut = cms.string("dR12 > 0.3"),
    filter = cms.bool(True)                                     
)

process.mixedTauPairsII = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedCaloTausLeadTrackPt12'),
    srcLeg2 = cms.InputTag('selectedPFTausLeadPionPt8'),
    dRmin12 = cms.double(0.),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

process.selectedMixedTauPairsII = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('mixedTauPairsII'),
    cut = cms.string("dR12 > 0.3"),
    filter = cms.bool(True)                                     
)

#--------------------------------------------------------------------------------
# keep event in case it passed either the caloTau + caloTau, pfTau + pfTau
# or mixed caloTau + pfTau selection
#--------------------------------------------------------------------------------

process.caloTauSkimPath = cms.Path(
    process.selectedCaloTausLeadTrackPt8 * process.selectedCaloTausLeadTrackPt12 
   * process.caloTauPairs
   * process.selectedCaloTauPairs
)

process.pfTauSkimPath = cms.Path(
    process.selectedPFTausLeadPionPt8 * process.selectedPFTausLeadPionPt12
   * process.pfTauPairs
   * process.selectedPFTauPairs
)

process.mixedTauSkimPathI = cms.Path(
    (process.selectedCaloTausLeadTrackPt8 + process.selectedPFTausLeadPionPt12)
   * process.mixedTauPairsI
   * process.selectedMixedTauPairsI
)

process.mixedTauSkimPathII = cms.Path(
    (process.selectedCaloTausLeadTrackPt12 + process.selectedPFTausLeadPionPt8)
   * process.mixedTauPairsII
   * process.selectedMixedTauPairsII
)

diTauEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pfTauSkimPath', 'caloTauSkimPath', 'mixedTauSkimPathI', 'mixedTauSkimPathII')
    )
)
process.diTauSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    tauAnalysisEventContent,                                               
    diTauEventSelection,
    fileName = cms.untracked.string('diTauSkim.root')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath( process.diTauSkimOutputModule )

