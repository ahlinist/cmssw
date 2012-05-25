import FWCore.ParameterSet.Config as cms

process = cms.Process("copyToCastor")

from TauAnalysis.Skimming.EventContent_cff import *

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START52_V9::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)

# Get all the skim files from the castor directory
#sourceFilePath = "/castor/cern.ch/user/f/friis/Run50plainharvest/"
#sourceFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/ZtoMuTau/GenZtoMuTauWithinAcc/"

##jobId = "2011Jun30v2"

##import TauAnalysis.Configuration.tools.castor as castor
##source_files = [ 'rfio:%s' % file_info['path'] for file_info in castor.nslsl(sourceFilePath) ]
##
##source_files_matched = []
##for source_file in source_files:
##    if source_file.find(jobId) != -1:
##	source_files_matched.append(source_file)
##print "source_files_matched", source_files_matched
##
##setattr(process.source, "fileNames", cms.untracked.vstring(source_files_matched))

dummyEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('')
    )
)

process.copyToCastorOutputModule = cms.OutputModule("PoolOutputModule",
    AODSIMEventContent,                                  
    fileName = cms.untracked.string(
        'simZplusJets_madgraph_AOD.root'
    ),
    maxSize = cms.untracked.int32(1000000000)                                                
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.copyToCastorOutputModule)

