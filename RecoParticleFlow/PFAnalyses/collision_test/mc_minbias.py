import FWCore.ParameterSet.Config as cms

process = cms.Process("REPROD")

from RecoParticleFlow.PFAnalyses.pflowOptions_cfi import *


#reconstruction
process.load("Configuration.StandardSequences.Simulation_cff")
#process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
#process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.load("Configuration.StandardSequences.RawToDigi_cff")
#process.load("Configuration.StandardSequences.DigiToRaw_cff")

process.GlobalTag.globaltag = "STARTUP3X_V12::All"
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True



readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       '/store/relval/CMSSW_3_4_0_pre5/RelValMinBias/GEN-SIM-RECO/STARTUP3X_V11-v1/0002/DA72BE71-08CC-DE11-87EA-0030487A1FEC.root',
       '/store/relval/CMSSW_3_4_0_pre5/RelValMinBias/GEN-SIM-RECO/STARTUP3X_V11-v1/0002/2434B1F7-F7CB-DE11-8CE1-0030487D1BCC.root',
       '/store/relval/CMSSW_3_4_0_pre5/RelValMinBias/GEN-SIM-RECO/STARTUP3X_V11-v1/0001/40851649-4DCB-DE11-A49C-001D09F251CC.root' ] );


secFiles.extend( [
               ] )


genEvents = 100
if options.kevents <> 0:
    genEvents = options.kevents * 1000
else:
    print 'Generating 100 events by default (alter with kevents=X option)'
    
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(genEvents)
)


process.particleFlow.pf_nsigma_HCAL = cms.double(1.0)
process.particleFlow.pf_calibMode = cms.uint32(2)
process.particleFlowBlock.pf_chi2_ECAL_HCAL = cms.double(100.0)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('Minbias_Tree_fast' + fileLabel)
)

import RecoParticleFlow.PFAnalyses.pflowCalibratable_cfi as calibratable

process.extractionToTree = cms.EDAnalyzer("ExtractionAnalyzer",
    calibratable.CollisionDelegate
)
process.extractionToTree.isMC=cms.bool(True)
process.extractionToTree.clustersFromCandidates=cms.bool(True)
process.extractionToTree.rechitsFromCandidates=cms.bool(False)


#process.extractionToEvent = cms.EDProducer("CalibratableProducer",
#    calibratable.CollisionDelegate         
#)


#Logging and verbosity
if options.batchMode <> 0 :
    process.load("FWCore.MessageLogger.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 100
else :
    print 'Not running in batch mode; Verbose output enabled...'
    process.MessageLogger = cms.Service("MessageLogger")
    process.MessageLogger.destinations=cms.untracked.vstring('cout')
    process.extractionToTree.debug = cms.int32(4)
    process.MessageLogger.debugModules=cms.untracked.vstring('*')
    
process.finishup = cms.OutputModule("PoolOutputModule",
    fileName=cms.untracked.string('Minbias_Events_fast' + fileLabel),
#    outputCommands=cms.untracked.vstring('keep *'),
    outputCommands=cms.untracked.vstring('drop *', 
                                         'keep *_particleFiltration_*_*', 
                                         'keep recoMuons_*_*_*', 
                                         'keep *_calibratable_*_*', 
                                         'keep *_faketracks_*_*', 
                                         'keep recoPFRecTracks_*_*_*', 
                                         'keep recoPFRecHits_*_*_*', 
                                         'keep recoPFClusters_*_*_*', 
                                         'keep recoPFBlocks_*_*_*', 
                                         'keep recoPFCandidates_*_*_*'),
    SelectEvents=cms.untracked.PSet(
         SelectEvents=cms.vstring('p1')
    )
)

process.particleFlowSimParticle.RecTracks = cms.InputTag("trackerDrivenElectronSeeds")

process.p1 = cms.Path(process.particleFlowSimParticle+
                      process.extractionToTree)

if options.outputCollections <> 0:
    process.outpath = cms.EndPath(process.finishup)


