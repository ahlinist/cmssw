## V-gamma Z(->mumu)gamma subskim
## Based on ElectroWeakAnalysis/Skimming/test/EWK_ZMuMuSubskim.py
## and PhysicsTools/PatAlgos/python/patTemplate_cfg.py

import FWCore.ParameterSet.Config as cms

process = cms.Process("SUBSKIM")

## Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

## Define the source
process.source = cms.Source("PoolSource",
     fileNames = cms.untracked.vstring(
      'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_5_7/RelValZMM/GEN-SIM-RECO/START3X_V26-v1/0012/10B71379-4549-DF11-9D80-003048D15D22.root'
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'START3X_V26::All'
process.load("Configuration.StandardSequences.MagneticField_cff")

## Load the PAT sequences first
process.load("PhysicsTools.PatAlgos.patSequences_cff")

## Load the ZMuMu Subskim paths - this may overwrite some of the PAT modules
process.load("ElectroWeakAnalysis.Skimming.dimuons_SkimPaths_cff")
## Turn off the ZMuMu filters
process.dimuonsFilter.minNumber = 0
process.dimuonsOneTrackFilter.minNumber = 0

## Add V-gamma specific sequence
process.load("ElectroWeakAnalysis.MultiBosons.Skimming.ZMuMuGammaSubskimSequences_cff")

## Import the output module configuration from the ZMuMu
from ElectroWeakAnalysis.Skimming.dimuonsOutputModule_cfi \
  import dimuonsOutputModule as zMuMuSubskimOutputModule
process.out = zMuMuSubskimOutputModule.clone(
  fileName = 'ZMuMuGammaPAT.root',
  SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring("MuMuGammasGlobal")
  )
)

## Add the PAT parameters to the output module
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out.outputCommands += cms.untracked.vstring(*patEventContent )

process.outpath = cms.EndPath(process.out)

## Ease the inspection of the file with `python -i <filename>'
if __name__ == "__main__":
  import user
