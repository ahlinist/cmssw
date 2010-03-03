import FWCore.ParameterSet.Config as cms

maxevents=100
isMC=False
#isMC=True


process = cms.Process('TEST')

process.load('JetMETAnalysis.PromptAnalysis.ntuple_cff')

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")

#MC (Summer09-V16D_900GeV-v1)
if (isMC):
    process.GlobalTag.globaltag ='START3X_V16D::All'

#DATA (Feb9ReReco)
else:
    process.GlobalTag.globaltag ='GR09_R_34X_V5::All'

process.load('Configuration.StandardSequences.Services_cff')
process.add_( cms.Service( "TFileService",
                           fileName = cms.string( 'your_output_PMTNoise.root' ),
                           closeFileFast = cms.untracked.bool(True)  ) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(maxevents) )
process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Feb9Skim_v1/0030/F893F078-3918-DF11-A63D-00304867BFAA.root'
    ),
    
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    
    secondaryFileNames = cms.untracked.vstring())

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.default.limit = 100
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
    'keep *_promptanaPMTnoise_*_*'
    ))

process.promptanaPMTnoise.isMC=isMC
process.promptanaPMTnoise.debug=0

process.theBigNtuple=cms.Path(#process.physDecl*
                              process.promptanaevent
                              *process.promptanaPMTnoise
                              *process.promptanaTree)

