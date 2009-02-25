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
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.edmDumpAnalysis_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleInteractionTMVAFilter_cfi")
process.singleInteractionTMVAFilter.CutOnClassifier = 0.9 

process.edmDumpAnalysisSingleVertex = process.edmDumpAnalysis.clone()

process.edmDumpAnalysisSingleInteraction = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisSingleInteraction.SaveROOTTree = True

process.edmDumpAnalysisSingleInteractionTMVA = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisSingleInteractionTMVA.SaveROOTTree = True

process.edmDumpAnalysisPileUp = process.edmDumpAnalysis.clone()
process.edmDumpAnalysisPileUp.SaveROOTTree = True

process.edmDumpAnalysisSingleVertexSingleInteraction = process.edmDumpAnalysis.clone()

process.edmDumpAnalysisSingleVertexPileUp = process.edmDumpAnalysis.clone()

process.add_(cms.Service("TFileService",
		#fileName = cms.string("analysisEdmDump_histos_SDWmunu_noPU.root")
                fileName = cms.string("analysisEdmDump_histos_SDWmunu_InitialLumPU.root")
                #fileName = cms.string("analysisEdmDump_histos_SDDijets_StageA43Bx.root")
	)
)

process.p1 = cms.Path(process.edmDumpAnalysis) 
process.p2 = cms.Path(process.singleVertexFilter*process.edmDumpAnalysisSingleVertex)
process.p3 = cms.Path(process.singleInteractionFilter*process.edmDumpAnalysisSingleInteraction)
process.p4 = cms.Path(~process.singleInteractionFilter*process.edmDumpAnalysisPileUp)
process.p5 = cms.Path(process.singleInteractionFilter*process.singleVertexFilter*process.edmDumpAnalysisSingleVertexSingleInteraction)
process.p6 = cms.Path(~process.singleInteractionFilter*process.singleVertexFilter*process.edmDumpAnalysisSingleVertexPileUp)

process.p7 = cms.Path(process.singleInteractionTMVAFilter*process.edmDumpAnalysisSingleInteractionTMVA)

process.schedule = cms.Schedule(process.p1,process.p2,process.p3,process.p4,process.p5,process.p6,process.p7)
