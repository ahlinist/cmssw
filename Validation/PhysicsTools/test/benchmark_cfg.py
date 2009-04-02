import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.load("DQMServices.Core.DQM_cfg")

###############################
# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATLayer0Summary')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    default          = cms.untracked.PSet( limit = cms.untracked.int32(0)  ),
    PATLayer0Summary = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Load geometry
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('IDEAL_V9::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

#############################


#UNCOMMENTED THIS
process.source = cms.Source("PoolSource",
#                            fileNames = cms.untracked.vstring('file:aod.root'
#                             fileNames = cms.untracked.vstring('/store/mc/Summer08/InclusiveMuPt15/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0060/0870D57E-93DE-DD11-BCD7-001A92971B08.root'
                             fileNames = cms.untracked.vstring('/store/mc/Summer08/ZJets-madgraph/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0002/88809B2F-77E4-DD11-AFF9-003048673F12.root'
                                                              )
                            )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# PAT Layer 0+1
process.load("PhysicsTools.PatAlgos.patLayer0_cff")
process.load("PhysicsTools.PatAlgos.patLayer1_cff")



#process.load("Validation.RecoParticleFlow.patBenchmarkGeneric_cff")
process.load("Validation.PhysicsTools.patObjectBenchmarkGeneric_cfi")

#process.patJetTag = cms.EDAnalyzer("PATValidation",
#    recoObject = cms.InputTag('jets'),
#    patObject = cms.InputTag( 'selectedLayer1Jets' ),
#    jetTag = cms.InputTag('softMuonTagInfos')
#)




process.p =cms.Path(
#    process.patBenchmarkGeneric*
    process.patLayer0*
    process.patLayer1*
    process.patJetTag*
    process.patMuonTag*
    process.patElectronTag*
    process.patMETTag*
    process.patTauTag*
    process.patPhotonTag	
   )

# load the pat layer 1 event content
process.load("PhysicsTools.PatAlgos.patLayer1_EventContent_cff")

# don't apply jet corrections
process.allLayer1Jets.addJetCorrFactors = False

process.schedule = cms.Schedule(process.p)

# ADDED THIS
process.out = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string('tree.root')
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet(
    makeTriggerResults = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(True),
    Rethrow = cms.untracked.vstring('Unknown', 
        'ProductNotFound', 
        'DictionaryNotFound', 
        'InsertFailure', 
        'Configuration', 
        'LogicError', 
        'UnimplementedFeature', 
        'InvalidReference', 
        'NullPointerError', 
        'NoProductSpecified', 
        'EventTimeout', 
        'EventCorruption', 
        'ModuleFailure', 
        'ScheduleExecutionFailure', 
        'EventProcessorFailure', 
        'FileInPathError', 
        'FatalRootError', 
        'NotFound')
)



#process.MessageLogger.cerr.FwkReport.reportEvery = 100


