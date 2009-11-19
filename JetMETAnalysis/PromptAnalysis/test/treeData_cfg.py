import FWCore.ParameterSet.Config as cms

process = cms.Process('TEST')
process.load('JetMETAnalysis.PromptAnalysis.ntuple_cff')

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.Services_cff')
process.add_( cms.Service( "TFileService",
                           fileName = cms.string( 'testTree.root' ),
                           closeFileFast = cms.untracked.bool(True)  ) )

#baseLocation="/store/data/Commissioning09/Calo/RECO/v8/000/116/736"

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )
process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring(
    "/store/relval/CMSSW_3_1_4/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0005/901ABD8A-E5B0-DE11-8AE6-000423D98DD4.root"),
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    
    secondaryFileNames = cms.untracked.vstring())

process.MessageLogger = cms.Service("MessageLogger",
                                    default = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(100)
    )
                                    )
