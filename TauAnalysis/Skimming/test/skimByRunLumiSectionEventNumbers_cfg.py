import FWCore.ParameterSet.Config as cms

import copy
import os
import re

process = cms.Process("skimByRunLumiSectionEventNumbers2")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START52_V9B::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(25000)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_5_2_x/skims/GoldenZmumu/2012Apr12/goldenZmumuEvents_ZplusJets_madgraph2_2012Apr12_AOD_183_2_KFf.root'
    )
)

# Get all the skim files from the castor directory
## import TauAnalysis.Configuration.tools.castor as castor
## inputFilePath = '/castor/cern.ch/user/v/veelken/CMSSW_5_2_x/skims/TauIdEffMeas/2012May12/'
## inputFileNames = []
## if inputFilePath.find('/castor/') != -1:
##     inputFileNames = [ 'rfio:%s' % file_info['path'] for file_info in castor.nslsl(inputFilePath) ]
## else:
##     inputFileNames = [ 'file:%s' % os.path.join(inputFilePath, file_name) for file_name in os.listdir(inputFilePath) ]
##
## sample = 'WplusJets_madgraph'
## jobId = '2012May12'
##
## print "inputFileNames = %s" % inputFileNames
##
## inputFileNames_matched = []
## inputFile_regex = \
##   r"[a-zA-Z0-9_/:.]*tauIdEffSample_%s_%s_AOD_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" % (sample, jobId)
## for inputFileName in inputFileNames:
##     inputFile_matcher = re.compile(inputFile_regex)
##     if inputFile_matcher.match(inputFileName):
## 	inputFileNames_matched.append(inputFileName)
##
## print "inputFileNames_matched = %s" % inputFileNames_matched
##
##setattr(process.source, "fileNames", cms.untracked.vstring(inputFileNames_matched))

##process.options = cms.untracked.PSet(
##    emptyRunLumiMode = cms.untracked.string('doNotHandleEmptyRunsAndLumis')
##)

process.selectEventsByRunLumiSectionEventNumber = cms.EDFilter("RunLumiSectionEventNumberFilter",
    runLumiSectionEventNumberFileName = cms.string(
        ##'/afs/cern.ch/work/c/calpas/CMSSW/FWliteHisto_v1_6_CaloMet20/debug_C1f.txt'
        ##'debug_C1f.txt'
        '/afs/cern.ch/user/v/veelken/scratch0/CMSSW_5_2_3_patch3/src/TauAnalysis/Test/test/debugMEtSys_selEvents.txt'
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
        #'/data1/veelken/CMSSW_5_2_x/skims/selEvents_bettysTauIdEff_WplusJets_madgraph_AOD.root'
        '/data1/veelken/CMSSW_5_2_x/skims/selEvents_debugMEtSys_ZplusJets_madgraph_AOD.root'                                        
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

