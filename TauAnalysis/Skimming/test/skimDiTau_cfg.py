import FWCore.ParameterSet.Config as cms

process = cms.Process("diTauSkim")

from TauAnalysis.Skimming.EventContent_cff import *

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_V12::All'
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
#
# Z --> tau tau (all decay modes; simulated with TAUOLA)
# 9k events CMSSW_2_2_3 RelVal sample
#
    '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0003/A4A3988A-BCCB-DD11-A103-001617E30E28.root',
    '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0003/D412FFFC-BCCB-DD11-8B20-000423D952C0.root',
    '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0003/F01E4F34-BDCB-DD11-B87D-001617C3B77C.root',
    '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0004/1CAA08F8-D3CB-DD11-ADF9-000423D6B358.root',
    '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0004/2800478C-08CC-DD11-94BB-0019B9F72BAA.root'
  )
)

#--------------------------------------------------------------------------------
# select tau-jets 
#--------------------------------------------------------------------------------

process.selectedPFTaus = cms.EDFilter("PFTauSelector",
  src = cms.InputTag('pfRecoTauProducer'),
  discriminators = cms.VPSet(
    cms.PSet(
      discriminator = cms.InputTag("pfRecoTauDiscriminationByLeadingPionPtCut"),
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
# combine selected tau-jets into pairs;
# require the two tau-jets to be separated by dR > 0.7 in eta-phi
#--------------------------------------------------------------------------------

process.caloTauPairs = cms.EDProducer("DiCandidatePairProducer",
  useLeadingTausOnly = cms.bool(False),
  srcLeg1 = cms.InputTag('selectedCaloTaus'),
  srcLeg2 = cms.InputTag('selectedCaloTaus'),
  dRmin12 = cms.double(0.3),
  srcMET = cms.InputTag(''),
  recoMode = cms.string(""),
  verbosity = cms.untracked.int32(0)                                       
)

process.selectedCaloTauPairs = cms.EDFilter("DiCandidatePairSelector",
  src = cms.InputTag('caloTauPairs'),
  cut = cms.string("dR12 > 0.7"),
  filter = cms.bool(True)                                     
)

process.pfTauPairs = cms.EDProducer("DiCandidatePairProducer",
  useLeadingTausOnly = cms.bool(False),
  srcLeg1 = cms.InputTag('selectedPFTaus'),
  srcLeg2 = cms.InputTag('selectedPFTaus'),
  dRmin12 = cms.double(0.3),
  srcMET = cms.InputTag(''),
  recoMode = cms.string(""),
  verbosity = cms.untracked.int32(0)
)

process.selectedPFTauPairs = cms.EDFilter("DiCandidatePairSelector",
  src = cms.InputTag('pfTauPairs'),
  cut = cms.string("dR12 > 0.7"),
  filter = cms.bool(True)                                     
)

process.mixedTauPairs = cms.EDProducer("DiCandidatePairProducer",
  useLeadingTausOnly = cms.bool(False),
  srcLeg1 = cms.InputTag('selectedCaloTaus'),
  srcLeg2 = cms.InputTag('selectedPFTaus'),
  dRmin12 = cms.double(0.3),
  srcMET = cms.InputTag(''),
  recoMode = cms.string(""),
  verbosity = cms.untracked.int32(0)
)

process.selectedMixedTauPairs = cms.EDFilter("DiCandidatePairSelector",
  src = cms.InputTag('mixedTauPairs'),
  cut = cms.string("dR12 > 0.7"),
  filter = cms.bool(True)                                     
)

#--------------------------------------------------------------------------------
# keep event in case it passed either the caloTau + caloTau, pfTau + pfTau
# or mixed caloTau + pfTau selection
#--------------------------------------------------------------------------------

process.caloTauSkimPath = cms.Path(
  process.selectedCaloTaus
 * process.caloTauPairs
 * process.selectedCaloTauPairs
)

process.pfTauSkimPath = cms.Path(
  process.selectedPFTaus
 * process.pfTauPairs
 * process.selectedPFTauPairs
)

process.mixedTauSkimPath = cms.Path(
  (process.selectedCaloTaus + process.selectedPFTaus)
 * process.mixedTauPairs
 * process.selectedMixedTauPairs
)

diTauEventSelection = cms.untracked.PSet(
  SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('pfTauSkimPath','caloTauSkimPath','mixedTauSkimPath')
  )
)
process.diTauSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
  tauAnalysisEventContent,                                               
  diTauEventSelection,
  fileName = cms.untracked.string('rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/diTauSkim.root')
)

process.options = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath( process.diTauSkimOutputModule )

