import FWCore.ParameterSet.Config as cms

process = cms.Process("CastorAnalysis")
# keep the logging output to a nice level ###
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.debugModules = cms.untracked.vstring('')
#process.MessageLogger.cerr.threshold = 'DEBUG'
process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.cerr.threshold = 'WARNING'
process.MessageLogger.categories.append('Analysis')
#process.MessageLogger.cerr.DEBUG = cms.untracked.PSet(
#    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
#    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(-1))
#)
#process.MessageLogger.cerr.INFO = cms.untracked.PSet(
#    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
#    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(-1))
#)

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       'file:/tmp/antoniov/POMWIG_SDWmunu_EdmDump_Castor_1.root',
       'file:/tmp/antoniov/POMWIG_SDWmunu_EdmDump_Castor_2.root',
       'file:/tmp/antoniov/POMWIG_SDWmunu_EdmDump_Castor_3.root',
       'file:/tmp/antoniov/POMWIG_SDWmunu_EdmDump_Castor_4.root', 
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.castorAnalysis = cms.EDAnalyzer("SimpleCastorAnalyzer",
    CastorGenInfoTag = cms.InputTag("castorGen"),
    CastorTowerInfoTag = cms.InputTag("castorTower"),
    GapSide = cms.int32(1),
    ThresholdIndex = cms.uint32(45),
    # If accessing directly the castor towers
    AccessCastorTowers = cms.bool(True),
    CastorTowersTag = cms.InputTag("CastorTowerReco"),
    TowerThreshold = cms.double(10.0) 
)

process.add_(cms.Service("TFileService",
		fileName = cms.string("analysisCastor_histos.root")
	)
)

process.p1 = cms.Path(process.castorAnalysis) 
