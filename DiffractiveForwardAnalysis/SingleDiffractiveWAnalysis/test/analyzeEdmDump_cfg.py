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

#from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.fileNames_POMWIG_SDPlusWmunu_noPU_cfi import filesPSet
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.fileNames_POMWIG_SDPlusWmunu_InitialLumiPU_cfi import filesPSet
#from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.fileNames_POMWIG_SDPlusDiJets_StageA43Bx_cfi import filesPSet

process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:/tmp/antoniov/POMWIG_SDPlusWmunu_EdmDump_InitialLumPU_2.root')
    filesPSet
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleInteractionFilter_cfi")

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import *
process.filter0PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 0)
process.filter1PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 1)
process.filter2PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 2)
process.filter3PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 3)
process.filter4PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 4)

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.edmDumpAnalysis_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleInteractionTMVAFilter_cfi")
process.singleInteractionTMVAFilter.CutOnClassifier = 0.7 

process.load("Validation.RecoVertex.validationPrimaryVertex_cff")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

process.edmDumpAnalysis.SaveROOTTree = True
process.edmDumpAnalysis.ThresholdIndexHF = 10

process.edmDumpAnalysisSingleInteraction = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisPileUp = process.edmDumpAnalysis.clone()

process.edmDumpAnalysisSingleVertex = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisWithAdaptiveVertexFitter = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisWithAdaptiveVertexFitter.VertexTag = "offlinePrimaryVerticesFromCTFTracksAVF"
process.edmDumpAnalysisWithKalmanVertexFitter = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisWithKalmanVertexFitter.VertexTag = "offlinePrimaryVerticesFromCTFTracksKVF"
process.edmDumpAnalysisWithPixelVertices = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisWithPixelVertices.VertexTag = "pixelVertices"

process.edmDumpAnalysis0PU = process.edmDumpAnalysis.clone()
process.edmDumpAnalysis1PU = process.edmDumpAnalysis.clone()
process.edmDumpAnalysis2PU = process.edmDumpAnalysis.clone()
process.edmDumpAnalysis3PU = process.edmDumpAnalysis.clone()
process.edmDumpAnalysis4PU = process.edmDumpAnalysis.clone()

process.edmDumpAnalysisSingleInteractionTMVA = process.edmDumpAnalysis.clone()

process.edmDumpAnalysisSingleVertexSingleInteraction = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisSingleVertexPileUp = process.edmDumpAnalysis.clone()

process.add_(cms.Service("TFileService",
		#fileName = cms.string("analysisEdmDump_SDWmunu_noPU.root")
                fileName = cms.string("analysisEdmDump_SDWmunu_InitialLumPU.root")
                #fileName = cms.string("analysisEdmDump_SDDijets_StageA43Bx.root")
	)
)

process.reco = cms.Path(process.vertexreco)
process.p1 = cms.Path(process.edmDumpAnalysis+process.edmDumpAnalysisWithAdaptiveVertexFitter+process.edmDumpAnalysisWithKalmanVertexFitter+process.edmDumpAnalysisWithPixelVertices) 
# Filter single-vertex events
process.p2 = cms.Path(process.singleVertexFilter*process.edmDumpAnalysisSingleVertex)
# Filter single-interaction (truth) events
process.p3 = cms.Path(process.singleInteractionFilter*process.edmDumpAnalysisSingleInteraction)
# Filter not single-interaction (truth) events
process.p4 = cms.Path(~process.singleInteractionFilter*process.edmDumpAnalysisPileUp)
# Filter single-interaction (truth) AND single-vertex events
process.p5 = cms.Path(process.singleInteractionFilter*process.singleVertexFilter*process.edmDumpAnalysisSingleVertexSingleInteraction)
# Filter not single-interaction (truth) events AND single-vertex events
process.p6 = cms.Path(~process.singleInteractionFilter*process.singleVertexFilter*process.edmDumpAnalysisSingleVertexPileUp)
# Filter single-interaction (from selection) events
process.p7 = cms.Path(process.singleInteractionTMVAFilter*process.edmDumpAnalysisSingleInteractionTMVA)

process.p8 = cms.Path(process.filter0PU*process.edmDumpAnalysis0PU)
process.p9 = cms.Path(process.filter1PU*process.edmDumpAnalysis1PU)
process.p10 = cms.Path(process.filter2PU*process.edmDumpAnalysis2PU)
process.p11 = cms.Path(process.filter3PU*process.edmDumpAnalysis3PU)
process.p12 = cms.Path(process.filter4PU*process.edmDumpAnalysis4PU)
