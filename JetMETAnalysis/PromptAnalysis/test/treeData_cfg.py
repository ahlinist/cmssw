import FWCore.ParameterSet.Config as cms
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

process = cms.Process('TEST')
process.load('JetMETAnalysis.PromptAnalysis.ntuple_cff')

process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
# process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
# process.load("Configuration/StandardSequences/RawToDigi_Data_cff")
# process.load("L1Trigger/Configuration/L1RawToDigi_cff")
# process.load("RecoMET/Configuration/RecoMET_BeamHaloId_cff")
# process.load("RecoMET/METProducers/BeamHaloSummary_cfi")
# process.load("RecoMET/METProducers/CSCHaloData_cfi")
# process.load("RecoMET/METProducers/EcalHaloData_cfi")
# process.load("RecoMET/METProducers/HcalHaloData_cfi")
# process.load("RecoMET/METProducers/GlobalHaloData_cfi")

# process.GlobalTag.globaltag ='GR09_R_34X_V5::All'

process.load("Configuration/StandardSequences/ReconstructionCosmics_cff")

process.load("RecoMuon/Configuration/RecoMuon_cff")

process.load('Configuration.StandardSequences.Services_cff')
process.add_( cms.Service( "TFileService",
                           fileName = cms.string( 'your_output_DATA.root' ),
                           closeFileFast = cms.untracked.bool(True)  ) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )
process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring(
    "/store/data/Commissioning10/MinimumBias/RECO/v4/000/130/910/1AACA102-D02F-DF11-A60F-0030487C6A66.root"
    ),
    
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    
    secondaryFileNames = cms.untracked.vstring())

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.default.limit = 100

# jet corrections
process.load("JetMETCorrections.Configuration.L2L3Corrections_900GeV_cff")
#process.load("JetMETCorrections.Configuration.L2L3Corrections_2360GeV_cff")

# summary
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

#process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
#from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed
#process.bit40OR41 = hltLevel1GTSeed.clone(L1TechTriggerSeeding = cms.bool(True), L1SeedsLogicalExpression = cms.string('40 OR 41'))

#from HLTrigger.HLTfilters.hltHighLevelDev_cfi import hltHighLevelDev
#process.physDecl = hltHighLevelDev.clone(HLTPaths = ['HLT_PhysicsDeclared'], HLTPathsPrescales = [1])

process.promptanaTree = cms.EDAnalyzer("PromptAnaTree",
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_promptanaevent_*_*',
    'keep *_promptanamet_*_*',
    'keep *_promptanatcmet_*_*',
    'keep *_promptanapfmet_*_*',
    'keep *_promptananohf_*_*',
    'keep *_promptanaic5calojet_*_*',
    #'keep *_promptanasc5calojet_*_*',
    #'keep *_promptanakt4calojet_*_*',
    'keep *_promptanaak5calojet_*_*',
    'keep *_promptanaJPTak5_*_*',
    'keep *_promptanaak5pfjet_*_*',
    #'keep *_promptanahalo_*_*',
    'keep *_promptanacalotowers_*_*',
    'keep *_promptanatrigger_*_*',
    'keep *_promptanavtx_*_*',
    'keep *_promptanatrack_*_*',
    'keep *_promptanacleanup_*_*'
    ))

process.theBigNtuple = cms.Path(
    #process.physDecl *
    #process.bit40OR41 *
    #process.BeamHaloId *
    (
    process.promptanaevent +
    process.promptanamet   +
    process.promptanatcmet   +
    process.promptanapfmet   +
    process.promptananohf  +
    process.promptanaic5calojet +
    process.promptanasc5calojet +
    #process.promptanakt4calojet +
    process.promptanaak5calojet +
    process.promptanaJPTak5 +
    process.promptanaak5pfjet +
    #process.promptanahalo +
    process.promptanacalotowers +
    process.promptanatrigger +
    process.promptanavtx +
    process.promptanatrack +
    process.promptanacleanup
    )
    * process.promptanaTree )

