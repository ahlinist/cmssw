import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process("skimByRunLumiSectionEventNumbersV3")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('GR_R_38X_V13A::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

castorFilePath = 'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/skims/tauCommissioning'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/selEvents_AHtoMuTau_HPSloose_2011Jan29_friis_RECO.root'
        'file:/data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/selEvents_AHtoMuTau_HPSloose_2011Feb03_bachtis_RECO.root'                       
    )                          
)

process.options = cms.untracked.PSet(
    emptyRunLumiMode = cms.untracked.string('doNotHandleEmptyRunsAndLumis')
)

process.selectEventsByRunLumiSectionEventNumber = cms.EDFilter("RunLumiSectionEventNumberFilter",
    runLumiSectionEventNumberFileName = cms.string(
        'selEvents_ZtoMuTau_bachtis_2011Feb03_converted.txt'
        #'uw_only_events.txt'                                            
    ),
    separator = cms.string(':')
)

process.skimPath = cms.Path( process.selectEventsByRunLumiSectionEventNumber )

eventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('skimPath')
    )
)

process.load("Configuration.EventContent.EventContent_cff")
process.origFEVTSIMEventContent = copy.deepcopy(process.FEVTSIMEventContent)
process.origFEVTSIMEventContent.outputCommands.extend(    
    cms.untracked.vstring(
        'drop LumiDetails_lumiProducer_*_*',
        'drop LumiSummary_lumiProducer_*_*',
        'drop RunSummary_lumiProducer_*_*',
        'drop *_MEtoEDMConverter_*_*',
        'drop *_*_*_skim*'
    )
)    

process.skimOutputModule = cms.OutputModule("PoolOutputModule",
    eventSelection,
    process.origFEVTSIMEventContent,
    fileName = cms.untracked.string(
        #'/data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/selEvents_AHtoMuTau_HPSloose_2011Feb03_bachtis_RECO.root'
        '/data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/uwOnlyEvents_AHtoMuTau_HPSloose_2011Feb03_bachtis_RECOslimmed.root'
    )
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
#__from TauAnalysis.Configuration.#recoSampleDefinitionsFileName# import *
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.selectEventsByRunEventNumber.runEventNumberFileName = cms.string('#runEventNumberFileName#')
#__process.skimOutputModule.fileName = cms.untracked.string('#outputFileName#')
#
#--------------------------------------------------------------------------------

process.o = cms.EndPath( process.skimOutputModule )

