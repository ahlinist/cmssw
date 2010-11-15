import os
import FWCore.ParameterSet.Config as cms
from ElectroWeakAnalysis.MultiBosons.Acceptance.inputFiles import inputFiles

process = cms.Process("ACCEPTANCE")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
## Options and Output Report
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

## Source
process.source = cms.Source("PoolSource",
    fileNames = inputFiles["/Zgamma/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO"]
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

basePath = "ElectroWeakAnalysis.MultiBosons.Acceptance"
process.load(basePath + ".ZMuMuGammaFilterSequence_cff")

process.p = cms.Path(process.acceptanceFilterSequence)
