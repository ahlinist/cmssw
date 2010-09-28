import FWCore.ParameterSet.Config as cms

process = cms.Process("EnergyShift")

## Declare input
from PhysicsTools.PatExamples.samplesDESY_cff import *

process.source = cms.Source("PoolSource",
  fileNames = ttbarJets
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32( -1 )
)

## Message logger configuration
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Select good jets
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import *
process.goodJets = selectedPatJets.clone(
    src="cleanPatJets",
    cut = 'abs(eta) < 3 & pt > 30. &'
    'emEnergyFraction > 0.01       &'
    'jetID.fHPD < 0.98             &'
    'jetID.n90Hits > 1'    
    )

## Analyze jets
from PhysicsTools.PatExamples.PatJetAnalyzer_cfi import analyzePatJets
process.Uncorrected = analyzePatJets.clone(src = 'goodJets', corrLevel='raw')
process.L2Relative  = analyzePatJets.clone(src = 'goodJets', corrLevel='rel')
process.L3Absolute  = analyzePatJets.clone(src = 'goodJets', corrLevel='abs')

## Define output file
process.TFileService = cms.Service("TFileService",
  fileName = cms.string('analyzeJetEnergyScale.root')
)

process.p = cms.Path(
    process.goodJets    * 
    process.Uncorrected *
    process.L3Absolute  *
    process.L2Relative
)
