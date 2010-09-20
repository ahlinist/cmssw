import FWCore.ParameterSet.Config as cms
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

process = cms.Process('USER')

process.load('JetMETAnalysis.PromptAnalysis.ntuple_cff')

process.load('Configuration.StandardSequences.Services_cff')
process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag ='GR_R_37X_V6A::All'
process.GlobalTag.globaltag ='GR10_P_V7::All'

process.load('Configuration.StandardSequences.Services_cff')
process.add_( cms.Service( "TFileService",
fileName = cms.string("JetMET__Run2010A-PromptReco-v4_tc30.root"),
                           closeFileFast = cms.untracked.bool(True)  ) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Run2010A/JetMETTau/RECO/v4/000/140/401/66925559-0C93-DF11-BEAD-00304879BAB2.root'
#    '/store/data/Run2010A/JetMETTau/RECO/v4/000/140/124/2E5222E1-518F-DF11-A722-0030487CD7EE.root'
#        'file:tcMetSkims.root'
        ),
    
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    
    secondaryFileNames = cms.untracked.vstring())

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.default.limit = 100

# summary
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# jet corrections
#process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_ReReco332_cff")
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

##################################################
############ SELECTIONS START HERE ###############
##################################################

# BPTX & BSC triggers filter
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND NOT (36 OR 37 OR 38 OR 39)')

# Primary vertex filter
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"),
    filter = cms.bool(True)
)

# Scraping filter
process.scrapingVeto = cms.EDFilter("FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
)

# Instead of rejecting the event, add a flag indicating the HBHE noise 
process.load('CommonTools/RecoAlgos/HBHENoiseFilterResultProducer_cfi')
process.hbheflag = cms.Path(process.HBHENoiseFilterResultProducer)

##################################################
############## SELECTIONS END HERE ###############
##################################################

process.promptanaTree = cms.EDAnalyzer("PromptAnaTree",
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_promptanaevent_*_*',
    'keep *_promptanamet_*_*',
    'keep *_promptanametdefault_*_*',
    'keep *_promptanatcmet_*_*',
    'keep *_promptanapfmet_*_*',
    'keep *_promptanaak5calojet_*_*',
    'keep *_promptanaak5pfjet_*_*',
    'keep *_promptanacalotowers_*_*',
    'keep *_promptanatrigger_*_*',
    'keep *_promptanavtx_*_*',
    'keep *_promptanatrack_*_*',
#    'keep *_promptanaecalspikes_*_*',
#    'keep *_promptanaPMTnoise_*_*',
    'keep *_HBHENoiseFilterResultProducer_*_*'
    ))

#process.promptanaPMTnoise.isMC=False
#process.promptanaPMTnoise.debug=0


# Filter sequence
process.filterSequence = cms.Sequence(
    process.primaryVertexFilter*
    process.scrapingVeto
)

process.ntuple_step=cms.Path(
    process.filterSequence*
    (
    process.promptanaevent +
    process.promptanamet   +
    process.promptanametdefault   +
    process.promptanatcmet   +
    process.promptanapfmet   +
    process.promptanaak5calojet +
    process.promptanaak5pfjet +
    process.promptanacalotowers +
    process.promptanatrigger +
    process.promptanavtx +
    process.promptanatrack +
    process.promptanaecalspikes +
#    process.promptanaPMTnoise
    )
    *process.promptanaTree
)

process.schedule = cms.Schedule(process.hbheflag, process.ntuple_step)

