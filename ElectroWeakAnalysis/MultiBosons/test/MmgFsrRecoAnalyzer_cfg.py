import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    '/store/mc/Winter10/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/GEN-SIM-RECO/E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/0033/FE75602D-4810-E011-B662-1CC1DE051038.root'
  )
)

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(500)
)

## Detector Conditions (needed for a EcalChannelStatusRcd)
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.PyReleaseValidation.autoCond import autoCond
process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )


## Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

## Add cleaning of collision data (no scraping events etc.)
##+ https://twiki.cern.ch/twiki/bin/viewauth/CMS/Collisions2010Recipes
basePath = "ElectroWeakAnalysis.MultiBosons.Skimming." # shorthand
process.load(basePath + "goodCollisionDataSequence_cff")
## Remove the hltPhysicsDeclared - it kills some good events, reference?
process.goodCollisionDataSequence.remove(process.hltPhysicsDeclared)

process.load("ElectroWeakAnalysis.MultiBosons.MmgFsrRecoAnalyzer_cfi")

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('MmgFsrRecoAnalyzer.root')
)

process.p = cms.Path(process.goodCollisionDataSequence + process.analyzeMmgFsr)

## Enable LogDebug for analyzeMmgFsr module
# process.MessageLogger.debugModules = ["analyzeMmgFsr"]
# process.MessageLogger.cerr.threshold = "DEBUG"

