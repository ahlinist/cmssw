import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Configuration to merge EDM skim output files into larger files
#
# To be used with createMergeConfigs.py script
#
#--------------------------------------------------------------------------------

process = cms.Process("elecTauSkimMerge")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
	'rfio:/castor/cern.ch/cms/user/j/jkolb/eTauSkims/bgEst/Ztautau/skimElecTau_1.root'
  )
)

process.count = cms.EDAnalyzer("EventCountAnalyzer")

process.outputModule = cms.OutputModule("PoolOutputModule",                                 
  fileName = cms.untracked.string('mergeElecTau_Ztautau_1.root')
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.outputModule.fileName = #outputFileName#
#
#--------------------------------------------------------------------------------

process.p = cms.Path(process.count)
process.o = cms.EndPath( process.outputModule )
