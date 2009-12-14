import FWCore.ParameterSet.Config as cms

process = cms.Process("ISOSKIM")

from RecoParticleFlow.PFAnalyses.pflowOptions_cfi import *
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = "MC_3XY_V12::All"

# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#from Configuration.StandardSequences.MagneticField_cff import *
process.load("Configuration.StandardSequences.MagneticField_0T_cff")
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
#process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True
##sourceDir = 'rfio:///castor/cern.ch/cms/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/'
sourceDir='rfio:///castor/cern.ch/cms/store/caf/user/malgeri/MinimumBias/rereco_bsc_v1/8c9b06949bc7320e4d50bd1f4bcbe723/'

q=range(1,17)
files = map(lambda x : sourceDir + 'MinBias122314_' + str(x) + '.root', q)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(files),
    inputCommands=cms.untracked.vstring('keep *', 'drop EBDataFramesSorted_*_*_*', 'drop EEDataFramesSorted_*_*_*')
       
)



genEvents = 100
if options.kevents <> 0:
    genEvents = options.kevents * 1000
else:
    print 'Generating 100 events by default (alter with kevents=X option)'
    
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(genEvents)
)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('IsoPions_Tree' + fileLabel)
)

import RecoParticleFlow.PFAnalyses.pflowCalibratable_cfi as calibratable


process.extractionToTree = cms.EDAnalyzer("ExtractionAnalyzer",
    calibratable.CollisionDelegate
)
process.extractionToTree.clustersFromCandidates=cms.bool(True)
process.extractionToTree.rechitsFromCandidates=cms.bool(False)
process.extractionToTree.isMC=cms.bool(False)


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
    fileName=cms.untracked.string('IsoPions_Events_' + fileLabel),
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

#process.load("RecoParticleFlow.PFTracking.particleFlowTrack_cff")
process.p1 = cms.Path(process.extractionToTree)

if options.outputCollections <> 0:
    process.outpath = cms.EndPath(process.finishup)


