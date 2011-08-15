import FWCore.ParameterSet.Config as cms

import copy
import os
import re

process = cms.Process("skimByRunLumiSectionEventNumbers")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START42_V12::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_0_b7bd.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_1_73be.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_2_84dd.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_3_f52e.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_4_22d4.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_5_c891.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_6_2a02.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_7_ba63.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_8_791c.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_9_2e07.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_10_4121.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_11_760b.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_12_7919.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_13_16f4.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_14_92f1.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_15_6813.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_16_d57c.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_17_0929.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_18_a9ab.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_19_2116.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_20_1f57.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_21_0b28.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_22_4c62.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_23_a08a.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_24_0b54.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_25_cf0e.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_26_440a.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_27_f05a.root', 
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/ZtoMuTau_tauIdEff/2011Jul23/skim_Ztautau_powheg_chunk_28_d383.root' 
    )
)

# Get all the skim files from the castor directory
import TauAnalysis.Configuration.tools.castor as castor
##inputFilePath = '/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/TauIdEffMeas/'
inputFilePath = '/data2/veelken/CMSSW_4_2_x/skims/TauTriggerStudy/user/v/veelken/CMSSW_4_2_x/skims/TauIdEffMeas/'
inputFileNames = []
if inputFilePath.find('/castor/') != -1:
    inputFileNames = [ 'rfio:%s' % file_info['path'] for file_info in castor.nslsl(inputFilePath) ]
else:
    inputFileNames = [ 'file:%s' % os.path.join(inputFilePath, file_name) for file_name in os.listdir(inputFilePath) ]

sample = 'Ztautau_powheg'
jobId = '2011Jul23'

#print "inputFileNames = %s" % inputFileNames

inputFileNames_matched = []
inputFile_regex = \
  r"[a-zA-Z0-9_/:.]*tauIdEffSample_%s_%s_RECO_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" % (sample, jobId)
for inputFileName in inputFileNames:
    inputFile_matcher = re.compile(inputFile_regex)
    if inputFile_matcher.match(inputFileName):
	inputFileNames_matched.append(inputFileName)

#print "inputFileNames_matched = %s" % inputFileNames_matched

setattr(process.source, "fileNames", cms.untracked.vstring(inputFileNames_matched))

process.options = cms.untracked.PSet(
    emptyRunLumiMode = cms.untracked.string('doNotHandleEmptyRunsAndLumis')
)

process.selectEventsByRunLumiSectionEventNumber = cms.EDFilter("RunLumiSectionEventNumberFilter",
    runLumiSectionEventNumberFileName = cms.string(
        '/data1/veelken/tmp/muonPtGt20/V6/selEvents_tauIdEff_Ztautau_powheg_C1p.txt'
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
        '/data1/veelken/CMSSW_4_2_x/skims/selEvents_Ztautau_powheg_tauIdEff_C1p_AOD.root'
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

