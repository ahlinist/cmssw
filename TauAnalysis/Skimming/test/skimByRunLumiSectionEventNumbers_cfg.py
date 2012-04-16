import FWCore.ParameterSet.Config as cms

import copy
import os
import re

process = cms.Process("skimByRunLumiSectionEventNumbers2")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START42_V13::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        ##'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/GoldenZmumu/simDYtoMuMu/DoubleMu_v2/goldenZmumuEvents_simDYtoMuMu_AOD_496_0_t1h.root',
        ##'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/GoldenZmumu/simDYtoMuMu/DoubleMu_v2/goldenZmumuEvents_simDYtoMuMu_AOD_497_0_GNy.root',
        ##'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/GoldenZmumu/simDYtoMuMu/DoubleMu_v2/goldenZmumuEvents_simDYtoMuMu_AOD_510_0_m4S.root'
        ##'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/TauIdEffMeas/2011Oct30/tauIdEffSample_Ztautau_powheg_2011Oct30_RECO_70_1_ynt.root',
        ##'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/TauIdEffMeas/2011Oct30/tauIdEffSample_Ztautau_powheg_2011Oct30_RECO_71_1_ugG.root'
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/TauFakeRate_WJets_RunB_fromArun/selEvents_Data_2011RunB_Wmunu_AOD.root'
    )
)

# Get all the skim files from the castor directory
##import TauAnalysis.Configuration.tools.castor as castor
##inputFilePath = '/data2/veelken/CMSSW_4_2_x/skims/TauTriggerStudy/user/v/veelken/CMSSW_4_2_x/skims/TauIdEffMeas/'
##inputFileNames = []
##if inputFilePath.find('/castor/') != -1:
##    inputFileNames = [ 'rfio:%s' % file_info['path'] for file_info in castor.nslsl(inputFilePath) ]
##else:
##    inputFileNames = [ 'file:%s' % os.path.join(inputFilePath, file_name) for file_name in os.listdir(inputFilePath) ]
##
##sample = 'Ztautau_powheg'
##jobId = '2011Jul23'
##
##print "inputFileNames = %s" % inputFileNames
##
##inputFileNames_matched = []
##inputFile_regex = \
##  r"[a-zA-Z0-9_/:.]*tauIdEffSample_%s_%s_RECO_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" % (sample, jobId)
##for inputFileName in inputFileNames:
##    inputFile_matcher = re.compile(inputFile_regex)
##    if inputFile_matcher.match(inputFileName):
##	inputFileNames_matched.append(inputFileName)
##
##print "inputFileNames_matched = %s" % inputFileNames_matched
##
##setattr(process.source, "fileNames", cms.untracked.vstring(inputFileNames_matched))

##process.options = cms.untracked.PSet(
##    emptyRunLumiMode = cms.untracked.string('doNotHandleEmptyRunsAndLumis')
##)

process.selectEventsByRunLumiSectionEventNumber = cms.EDFilter("RunLumiSectionEventNumberFilter",
    runLumiSectionEventNumberFileName = cms.string(
        #'/afs/cern.ch/user/v/veelken/scratch0/CMSSW_4_2_4_patch1/src/TauAnalysis/Test/test/selEvents_checkMEtSmearing_unclusteredEnDown.txt'
        #'/afs/cern.ch/user/v/veelken/scratch0/CMSSW_4_2_4_patch1/src/TauAnalysis/TauIdEfficiency/test/selEvents_Ztautau_tauIdPassed_but_loosePFIsoFailed.txt'
        #'/afs/cern.ch/user/v/veelken/scratch0/CMSSW_4_2_4_patch1/src/TauAnalysis/Skimming/test/TauFakeRate_WJet_TestSampleNVtX15_COR2_HPS0.root_debug.txt'
        '/afs/cern.ch/user/v/veelken/scratch0/CMSSW_4_2_4_patch1/src/TauAnalysis/Skimming/test/debug-passed-nvtx20.txt'
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
##process.origFEVTSIMEventContent.outputCommands.extend(    
##    cms.untracked.vstring(
##        'drop LumiDetails_lumiProducer_*_*',
##        'drop LumiSummary_lumiProducer_*_*',
##        'drop RunSummary_lumiProducer_*_*',
##        'drop *_MEtoEDMConverter_*_*',
##        'drop *_*_*_skim*',
##        'keep *_*_*_*' # CV: only for Testing !!
##    )
##)    

process.skimOutputModule = cms.OutputModule("PoolOutputModule",
    eventSelection,
    process.origFEVTSIMEventContent,
    fileName = cms.untracked.string(
        #'/data1/veelken/CMSSW_4_2_x/skims/selEvents_checkMEtSmearing_unclusteredEnDown_AOD.root'
        #'/data1/veelken/CMSSW_4_2_x/skims/selEvents_Ztautau_tauIdPassed_but_loosePFIsoFailed_AOD.root'
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/TauFakeRate_WJets_RunB_fromArun/selEvents_Data_2011RunB_Wmunu_AOD_numVerticesEq20.root'
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

process.o = cms.EndPath(process.skimOutputModule)

