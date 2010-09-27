import FWCore.ParameterSet.Config as cms

process = cms.Process("runSPlotAnalysis")

process.load('FWCore/MessageService/MessageLogger_cfi')
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.runSPlotAnalysis = cms.EDAnalyzer("SPlotAnalyzer",

)

process.p = cms.Path(process.runSPlotAnalysis)
