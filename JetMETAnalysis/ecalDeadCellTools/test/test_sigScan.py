import FWCore.ParameterSet.Config as cms

process = cms.Process('sigScan')

process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/RawToDigi_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
from Configuration.PyReleaseValidation.autoCond import autoCond
process.GlobalTag.globaltag = 'GR_R_38X_V13A::All'

process.load('Configuration.StandardSequences.Services_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()

# summary
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.source = cms.Source ("PoolSource", fileNames = readFiles, secondaryFileNames = secFiles, duplicateCheckMode = cms.untracked.string("noDuplicateCheck"))

readFiles.extend( [
       'file:MetSig_skim_JetMET_Run2010A_Sep17ReReco.root',
] );

secFiles.extend( [
] );

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.MessageLogger.cerr.default.limit = 100

# summary
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')

process.noscraping = cms.EDFilter(
  "FilterOutScraping",
  applyfilter = cms.untracked.bool(True),
  debugOn = cms.untracked.bool(False),
  numtrack = cms.untracked.uint32(10),
  thresh = cms.untracked.double(0.25)
)

process.goodVertices = cms.EDFilter(
  "VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)

process.basicEvtSel_seq = cms.Sequence(
  process.HBHENoiseFilter *
  process.noscraping *
  process.goodVertices
)

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
process.hltFilter = hlt.hltHighLevel.clone(
   TriggerResultsTag = cms.InputTag("TriggerResults","","REDIGI38X"),
   HLTPaths = cms.vstring("HLT_Jet30U"),
   throw = True, # Don't throw?!
   andOr = True
)

# jet corrections
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

# met corrections
process.load('JetMETCorrections.Type1MET.MetType1Corrections_cff')
process.metJESCorAK5CaloJet.useTypeII = True # CaloMET Type-II
process.metJESCorAK5PFJet.useTypeII = False # Type-I PfMET

process.load('JetMETAnalysis.simpleDRfilter.simpleDRfilter_cfi')
process.simpleDRfilter.debug = cms.untracked.bool(True)
process.simpleDRfilter.jetInputTag = cms.InputTag("ak5PFJetsL2L3")
process.simpleDRfilter.metInputTag = cms.InputTag("metJESCorAK5PFJet")
process.simpleDRfilter.doFilter = cms.untracked.bool(True) # to enable filter or not

process.simpleDRfilter.makeProfileRoot = True

process.corrJet_seq = cms.Sequence(process.ak5CaloJetsL2L3+process.ak5PFJetsL2L3)
process.corrMet_seq = cms.Sequence(process.metJESCorAK5CaloJet+process.metJESCorAK5PFJet)

process.theANApath = cms.Path(process.basicEvtSel_seq * process.corrJet_seq * process.corrMet_seq * process.simpleDRfilter)
