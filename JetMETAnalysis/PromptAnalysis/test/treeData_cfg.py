import FWCore.ParameterSet.Config as cms
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

process = cms.Process('TEST')
process.load('JetMETAnalysis.PromptAnalysis.ntuple_cff')

process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
process.load("Configuration/StandardSequences/RawToDigi_Data_cff")
process.load("L1Trigger/Configuration/L1RawToDigi_cff")
process.load("RecoMET/Configuration/RecoMET_BeamHaloId_cff")
process.load("RecoMET/METProducers/BeamHaloSummary_cfi")
process.load("RecoMET/METProducers/CSCHaloData_cfi")
process.load("RecoMET/METProducers/EcalHaloData_cfi")
process.load("RecoMET/METProducers/HcalHaloData_cfi")
process.load("RecoMET/METProducers/GlobalHaloData_cfi")
process.GlobalTag.globaltag ='STARTUP31X_V7::All'

process.load("Configuration/StandardSequences/ReconstructionCosmics_cff")

process.load("RecoMuon/Configuration/RecoMuon_cff")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.Services_cff')
process.add_( cms.Service( "TFileService",
                           fileName = cms.string( 'BeamCommissioning09__ExpressPhysics_123151_new.root' ),
                           closeFileFast = cms.untracked.bool(True)  ) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring(
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/0E45A7CE-F5DD-DE11-9B2E-001617E30CC8.root"
        ),
    
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    
    secondaryFileNames = cms.untracked.vstring())

process.MessageLogger = cms.Service("MessageLogger",
                                    default = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(100)
    )
                                    )

process.halo = cms.Path( process.gtDigis
                      * process.l1GtRecord
                      * process.BeamHaloId
                      )

process.promptanaTree = cms.EDAnalyzer("PromptAnaTree",
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_promptanaevent_*_*',
    'keep *_promptanamet_*_*',
    'keep *_promptanatcmet_*_*',
    'keep *_promptananohf_*_*',
    'keep *_promptanaic5calojet_*_*',
    'keep *_promptanasc5calojet_*_*',
    'keep *_promptanakt4calojet_*_*',
    'keep *_promptanahalo_*_*',
    'keep *_promptanacalotowers_*_*',
    'keep *_promptanatrigger_*_*',
    'keep *_promptanavtx_*_*',
    'keep *_promptanatrack_*_*',
    'keep *_promptanacleanup_*_*'
    ))

process.theBigNtuple = cms.Path( (
    process.promptanaevent +
    process.promptanamet   +
    process.promptanatcmet   +
    process.promptananohf  +
    process.promptanaic5calojet +
    process.promptanasc5calojet +
    process.promptanakt4calojet +
    process.promptanahalo +
    process.promptanacalotowers +
    process.promptanatrigger +
    process.promptanavtx +
    process.promptanatrack +
    process.promptanacleanup
    ) * process.promptanaTree )

schedule = cms.Schedule( process.halo, process.theBigNtuple )
