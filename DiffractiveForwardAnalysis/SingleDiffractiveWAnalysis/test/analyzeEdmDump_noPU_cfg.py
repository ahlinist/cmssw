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
#from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.fileNames_PYTHIA6_Wmunu_noPU_cfi import filesPSet

process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:/tmp/antoniov/POMWIG_SDPlusWmunu_EdmDump_InitialLumPU_2.root')
    filesPSet
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleInteractionFilter_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.edmDumpAnalysis_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleInteractionTMVAFilter_cfi")
process.singleInteractionTMVAFilter.CutOnClassifier = 0.7 

process.load("Validation.RecoVertex.validationPrimaryVertex_cff")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

process.edmDumpAnalysis.AccessPileUpInfo = False
process.edmDumpAnalysis.SaveROOTTree = True
process.edmDumpAnalysis.ThresholdIndexHF = 10

process.edmDumpAnalysisSingleVertex = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisWithAdaptiveVertexFitter = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisWithAdaptiveVertexFitter.VertexTag = "offlinePrimaryVerticesFromCTFTracksAVF"
process.edmDumpAnalysisWithKalmanVertexFitter = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisWithKalmanVertexFitter.VertexTag = "offlinePrimaryVerticesFromCTFTracksKVF"
process.edmDumpAnalysisWithPixelVertices = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisWithPixelVertices.VertexTag = "pixelVertices"

process.edmDumpAnalysisSingleInteractionTMVA = process.edmDumpAnalysis.clone()

process.add_(cms.Service("TFileService",
		fileName = cms.string("analysisEdmDump_SDWmunu_noPU.root")
                #fileName = cms.string("analysisEdmDump_Wmunu_noPU.root")
	)
)

process.reco = cms.Path(process.vertexreco)
#process.p1 = cms.Path(process.edmDumpAnalysis+process.edmDumpAnalysisWithAdaptiveVertexFitter+process.edmDumpAnalysisWithKalmanVertexFitter+process.edmDumpAnalysisWithPixelVertices) 
process.p1 = cms.Path(process.edmDumpAnalysis+process.edmDumpAnalysisWithAdaptiveVertexFitter+process.edmDumpAnalysisWithKalmanVertexFitter)
# Filter single-vertex events
process.p2 = cms.Path(process.singleVertexFilter*process.edmDumpAnalysisSingleVertex)
# Filter single-interaction (from MVA selection) events
process.p3 = cms.Path(process.singleInteractionTMVAFilter*process.edmDumpAnalysisSingleInteractionTMVA)

process.schedule = cms.Schedule(process.reco,process.p1,process.p2,process.p3)
