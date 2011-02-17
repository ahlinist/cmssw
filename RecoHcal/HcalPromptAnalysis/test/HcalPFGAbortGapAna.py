import FWCore.ParameterSet.Config as cms

process = cms.Process("ANA")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

####################### configure pool source #############################

# Meant to be run on TestEnables stream:
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    '/store/data/Commissioning11/TestEnables/RAW/v1/000/157/884/FEB586C7-C938-E011-B880-00304879FC6C.root'
    )
                            )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000))

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START310_V2::All'

######################## Configure Analyzer ###############################

process.ana = cms.EDAnalyzer("HcalPFGAbortGapAna")

#process.TFileService = cms.Service("TFileService",
#    fileName = cms.string('junk.root')
#)


process.analysis_step       = cms.Path(process.ana)
process.schedule = cms.Schedule(process.analysis_step)

