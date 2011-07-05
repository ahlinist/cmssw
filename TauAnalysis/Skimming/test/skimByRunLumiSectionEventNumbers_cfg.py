import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process("skimByRunLumiSectionEventNumbersV4")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('GR_R_38X_V15::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

castorFilePath = 'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/skims/tauCommissioning'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/veelken/CMSSW_4_2_x/skims/skimGenZtoMuTauWithinAcc_Ztautau_2011Jun30v2_AOD.root'
    )
)

## Get all the skim files from the castor directory
##import TauAnalysis.Configuration.tools.castor as castor
##sourceFilePath = "/castor/cern.ch/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run07/"
##source_files = [ 'rfio:%s' % file_info['path'] for file_info in castor.nslsl(sourceFilePath) ]
##
##jobId = "final_events_ZtoElecTau_data"
##
##source_files_matched = []
##
##for source_file in source_files:
##    if source_file.find(jobId) != -1:
##	source_files_matched.append(source_file)
##
##print "source_files_matched", source_files_matched
##
##setattr(process.source, "fileNames", cms.untracked.vstring(source_files_matched))

process.options = cms.untracked.PSet(
    emptyRunLumiMode = cms.untracked.string('doNotHandleEmptyRunsAndLumis')
)

process.selectEventsByRunLumiSectionEventNumber = cms.EDFilter("RunLumiSectionEventNumberFilter",
    runLumiSectionEventNumberFileName = cms.string(
        '/afs/cern.ch/user/v/veelken/scratch0/CMSSW_4_2_4_patch1/src/TauAnalysis/Test/test/selEvent_genZtoMuTau.txt'
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
        '/data1/veelken/CMSSW_4_2_x/skims/skimGenZtoMuTauWithinAcc_Ztautau_2011Jun30v2_C1sel_AOD.root'
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

