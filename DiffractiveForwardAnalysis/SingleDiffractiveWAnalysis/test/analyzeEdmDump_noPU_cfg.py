import FWCore.ParameterSet.Config as cms

process = cms.Process("EdmDumpAnalysis")
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

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.fileNames_POMWIG_SDPlusWmunu_noPU_cfi import filesPSet
#from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.fileNames_POMWIG_SDPlusWmunu_InitialLumiPU_cfi import filesPSet
#from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.fileNames_POMWIG_SDPlusDiJets_StageA43Bx_cfi import filesPSet

process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:/tmp/antoniov/POMWIG_SDPlusWmunu_EdmDump_InitialLumPU_2.root')
    filesPSet
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.singleVertexFilter = cms.EDFilter("SingleVertexFilter",
    VertexTag = cms.InputTag("offlinePrimaryVertices")
)

process.edmDumpAnalysis = cms.EDAnalyzer("EdmDumpAnalyzer",
    CastorGenInfoTag = cms.InputTag("castorGen"),
    CastorTowerInfoTag = cms.InputTag("castorTower"),
    GapSide = cms.int32(1),
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    ThresholdIndexHF = cms.uint32(22),
    ThresholdIndexCastor = cms.uint32(25),
    # If accessing directly the castor towers
    AccessCastorTowers = cms.bool(False),
    CastorTowersTag = cms.InputTag("CastorTowerReco"),
    TowerThreshold = cms.double(15.0),
    # If accessing the pile-up info
    AccessPileUpInfo = cms.bool(False),  
    # Saves or not ROOT TTree
    SaveROOTTree = cms.untracked.bool(True)
)

process.edmDumpAnalysisSingleVertex = process.edmDumpAnalysis.clone()

process.add_(cms.Service("TFileService",
		fileName = cms.string("analysisEdmDump_histos_SDWmunu_noPU.root")
	)
)

process.p1 = cms.Path(process.edmDumpAnalysis) 
process.p2 = cms.Path(process.singleVertexFilter*process.edmDumpAnalysisSingleVertex)

process.schedule = cms.Schedule(process.p1,process.p2)
