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
                           fileName = cms.string( 'testTree.root' ),
                           closeFileFast = cms.untracked.bool(True)  ) )

#baseLocation="/store/data/Commissioning09/Calo/RECO/v8/000/116/736"

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring(
    ##Calo from Commissioning09
    '/store/data/Commissioning09/Calo/RECO/v9/000/118/969/FEA310F0-0BC6-DE11-B62C-001D09F29524.root',
    '/store/data/Commissioning09/Calo/RECO/v9/000/118/969/FC1F74B6-DBC5-DE11-A2A0-001D09F2516D.root',
    '/store/data/Commissioning09/Calo/RECO/v9/000/118/969/FA7C1BCC-0EC6-DE11-A0DF-001D09F29146.root',
    '/store/data/Commissioning09/Calo/RECO/v9/000/118/969/FA214C80-1BC6-DE11-8A68-001D09F250AF.root',
    '/store/data/Commissioning09/Calo/RECO/v9/000/118/969/F2E60926-D8C5-DE11-A825-0030487A3232.root',
    '/store/data/Commissioning09/Calo/RECO/v9/000/118/969/E87A8457-01C6-DE11-9553-001D09F2AD84.root',
    '/store/data/Commissioning09/Calo/RECO/v9/000/118/969/E8477624-F3C5-DE11-B084-001D09F290CE.root',
    '/store/data/Commissioning09/Calo/RECO/v9/000/118/969/E4629D10-ECC5-DE11-8DE7-001D09F276CF.root',
    '/store/data/Commissioning09/Calo/RECO/v9/000/118/969/E22A18FD-F5C5-DE11-8184-001D09F2527B.root',
    '/store/data/Commissioning09/Calo/RECO/v9/000/118/969/D82C3D52-06C6-DE11-81E0-000423D98C20.root'
    ## ExpressPhysics from BeamCommissioning09
    #"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/0C3D08C3-2AD6-DE11-A4C0-001D09F251B8.root",
    #"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/0A520AE5-20D6-DE11-9591-001D09F23174.root",
    #"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/047C5FF1-27D6-DE11-A9EE-001D09F28E80.root"
    ),
    
    #    "/store/relval/CMSSW_3_1_4/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0005/901ABD8A-E5B0-DE11-8AE6-000423D98DD4.root"),
    #duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    
    secondaryFileNames = cms.untracked.vstring())

process.MessageLogger = cms.Service("MessageLogger",
                                    default = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(100)
    )
                                    )

process.halo = cms.Path( process.gtDigis
                      * process.l1GtRecord
                      * process.BeamHaloId
                      * process.CSCHaloData
                      * process.EcalHaloData
                      * process.HcalHaloData
                      * process.GlobalHaloData
                      * process.BeamHaloSummary
                      )

process.promptanaTree = cms.EDAnalyzer("PromptAnaTree",
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_promptanaevent_*_*',
    'keep *_promptanamet_*_*',
    'keep *_promptananohf_*_*',
    'keep *_promptanaic5calojet_*_*',
    'keep *_promptanakt4calojet_*_*',
    'keep *_promptanahalo_*_*',
    'keep *_promptanacalotowers_*_*'
    ))

process.theBigNtuple = cms.Path( (
    process.promptanaevent +
    process.promptanamet   +
    process.promptananohf  +
    process.promptanaic5calojet +
    process.promptanakt4calojet +
    process.promptanahalo +
    process.promptanacalotowers 
    ) * process.promptanaTree )

schedule = cms.Schedule( process.halo, process.theBigNtuple )
