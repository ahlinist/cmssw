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

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/POMWIG_SDPlusWmunu_EdmDump_InitialLumPU_2.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.offlinePrimaryVertices_cff")
process.reco = cms.Path(process.vertexreco)

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleInteractionFilter_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleInteractionTMVAFilter_cfi")
process.singleInteractionTMVAFilter.CutOnClassifier = 0.4
process.singleInteractionTMVAFilterTight = process.singleInteractionTMVAFilter.clone(CutOnClassifier = 0.8)

filters = ['singleVertexFilter','singleInteractionTMVAFilter','singleInteractionTMVAFilterTight']

vertexAlgos = ['pixelVertices',
               'offlinePrimaryVerticesFromCTFTracksAVFModified']

attributes = {'VertexTag':vertexAlgos}

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.edmDumpAnalysis_cfi")
process.edmDumpAnalysis.AccessPileUpInfo = False
process.edmDumpAnalysis.SaveROOTTree = True
process.edmDumpAnalysis.ThresholdIndexHF = 12

makeAnalysis(process,'edmDumpAnalysis',attributes,filters)

process.add_(cms.Service("TFileService",
		fileName = cms.string("analysisEdmDump_SDWmunu_noPU.root")
                #fileName = cms.string("analysisEdmDump_Wmunu_noPU.root")
	)
)
