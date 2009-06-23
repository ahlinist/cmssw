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
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/edmDump_WMuNu.root')
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

process.pileUpFilter = cms.Sequence(~process.singleInteractionFilter)
process.singleInteractionTMVA_SingleInteraction = cms.Sequence(process.singleInteractionFilter+process.singleInteractionTMVAFilter)
process.singleInteractionTMVATight_SingleInteraction = cms.Sequence(process.singleInteractionFilter+process.singleInteractionTMVAFilterTight)

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import *
listFiltersPU = []
for i in range(5):
    listFiltersPU.append('filter%dPU'%i)
    setattr(process,'filter%dPU'%i,pileUpNumberFilter.clone(NumberOfPileUpEvents = i))

filters = ['singleInteractionFilter','!singleInteractionFilter','pileUpFilter','singleVertexFilter','singleInteractionTMVAFilter','singleInteractionTMVAFilterTight','singleInteractionTMVA_SingleInteraction','singleInteractionTMVATight_SingleInteraction']
filters.extend(listFiltersPU)

attributes = [{'VertexTag':'pixelVertices'},
              {'VertexTag':'offlinePrimaryVerticesFromCTFTracksAVFModified'}]

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.edmDumpAnalysis_cfi")
process.edmDumpAnalysis.SaveROOTTree = True
process.edmDumpAnalysis.ThresholdIndexHF = 12

makeAnalysis(process,'edmDumpAnalysis',attributes,filters)

process.add_(cms.Service("TFileService",
		fileName = cms.string("analysisEdmDump_SDWmunu.root")
                #fileName = cms.string("analysisEdmDump_Wmunu.root")
	)
)
