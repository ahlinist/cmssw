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
        #'file:/data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/selEvents_AHtoMuTau_HPSloose_2011Jan29_friis_RECO.root'
        #'file:/data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/selEvents_AHtoMuTau_HPSloose_2011Feb03_bachtis_RECO.root'
        #'file:/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau/selEvents_ZtoMuTau_HPSloose_2011Feb03_veelken_RECO.root'
        #'/store/data/Run2010A/Mu/RECO/Nov4ReReco_v1/0001/E82DC3B2-1AEB-DF11-B51A-0017A477102C.root',
        #'/store/data/Run2010B/Mu/RECO/Nov4ReReco_v1/0000/641FB9B3-61EA-DF11-A147-E0CB4E29C4FB.root',
        #'/store/data/Run2010B/Mu/RECO/Nov4ReReco_v1/0000/CC42DC6D-08EA-DF11-A05B-E0CB4E29C4CA.root',
        #'/store/data/Run2010B/Mu/RECO/Nov4ReReco_v1/0001/FAA6E118-F8E9-DF11-9B42-E0CB4E553637.root',
        #'/store/data/Run2010B/Mu/RECO/Nov4ReReco_v1/0001/EC0DA164-48EA-DF11-9B81-E0CB4E29C51A.root',
        #'/store/data/Run2010B/Mu/RECO/Nov4ReReco_v1/0000/7A1972FA-10EA-DF11-876D-485B39800BF0.root',
        #'/store/data/Run2010B/Mu/RECO/Nov4ReReco_v1/0000/644B8C0A-57EA-DF11-895E-E0CB4E19F975.root'
        'file:/data1/friis/CMSSW_3_8_x/skims/ZtoMuTau/skim_Ztautau_powheg_chunk_8_fcc0.root'                        
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
        #'/afs/cern.ch/user/v/veelken/public/selEvents_ZtoMuTau_2011Feb01_HPSloose_twoOSglobalMuons.txt'
        #'selEvents_ZtoMuTau_bachtis_2011Feb03_converted.txt'
        #'uw_only_events.txt'
        #'selEvents_ZtoMuTau_approval_2011Mar03_converted.txt'
        #'selEvents_ZtoElecTau_approval_2011Mar03_converted.txt'
        #'selEvents_ZtoMuMu_approval_2011Mar03.txt'
        'selEvents_ZtautauPU156bx.txt'                                                           
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
        #'/data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/uwOnlyEvents_AHtoMuTau_HPSloose_2011Feb03_bachtis_RECOslimmed.root'
        #'/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau/selEvents_ZtoMuTau_HPSloose_2011Feb03_veelken_twoOSglobalMuons_RECO.root'
        #'/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau/selEvents_ZtoMuTau_approval_2011Mar03_RECO.root'
        #'/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau/selEvents_ZtoElecTau_approval_2011Mar03_RECO.root'
        #'/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau/selEvents_ZtoMuMu_approval_2011Mar03_RECO.root'
        '/data1/veelken/CMSSW_3_8_x/skims/test/selEvents_ZtautauPU156bx_RECO.root'                                        
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

