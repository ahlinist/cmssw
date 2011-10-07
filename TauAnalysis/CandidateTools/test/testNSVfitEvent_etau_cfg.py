import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    "file:patTuple.root"
  )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.MessageLogger = cms.Service("MessageLogger")

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('analyzeNSVfit.root')
)

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.PyReleaseValidation.autoCond import autoCond
process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
process.load("Configuration.StandardSequences.MagneticField_cff")


## load nSVfitProducer (plugin version)
process.load("TauAnalysis.CandidateTools.nSVfitAlgorithmDiTau_cfi")

process.nSVfitProducerByLikelihoodMaximization.config.event.resonances.A.daughters.leg1.src = "cleanPatElectrons"
process.nSVfitProducerByLikelihoodMaximization.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitElectronLikelihoodPhaseSpace)
process.nSVfitProducerByLikelihoodMaximization.config.event.resonances.A.daughters.leg1.builder = process.nSVfitTauToElecBuilder


process.nSVfitProducerByLikelihoodMaximization.config.event.resonances.A.daughters.leg2.src = "cleanPatTaus"
process.nSVfitProducerByLikelihoodMaximization.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace)
process.nSVfitProducerByLikelihoodMaximization.config.event.resonances.A.daughters.leg2.builder = process.nSVfitTauToHadBuilder

process.nSVfitProducerByLikelihoodMaximization.config.event.srcMEt = "patMETs"
process.nSVfitProducerByLikelihoodMaximization.algorithm.verbosity = 0


## load nSVfitEventAnalyzer
process.load("TauAnalysis.CandidateTools.nSVfitEventAnalyzer_cfi")
process.nSVfitEventAnalyzer.leps1 = "cleanPatElectrons"
process.nSVfitEventAnalyzer.leps2 = "cleanPatTaus"
process.nSVfitEventAnalyzer.type1 = "lep"
process.nSVfitEventAnalyzer.type2 = "had"

## check the event content
process.content = cms.EDAnalyzer("EventContentAnalyzer")

process.p = cms.Path(
#   process.nSVfitProducerByLikelihoodMaximization
# * process.content
    process.nSVfitEventAnalyzer
    )
