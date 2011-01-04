import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Write run + luminosity section + event numbers of events selected in FilterStatistics into ASCII file
# (format = three columns of numbers separated by white-space character (space or tab);
#           with the run#/luminosity section#/event# specified in the first/second/third column)
#--------------------------------------------------------------------------------

process = cms.Process('dumpRunLumiSectionEventNumbers')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.loadFilterStatistics = cms.EDAnalyzer("DQMFileLoader",
    all = cms.PSet(
        inputFileNames = cms.vstring(
            '/data1/veelken/CMSSW_3_8_x/plots/AHtoMuTau/2010Dec23_lxbatch/plotsAHtoMuTau_all.root'
        ),
        dqmDirectory_store = cms.string('/')
    )
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.dumpRunLumiSectionEventNumbers = cms.EDAnalyzer("DQMDumpRunLumiSectionEventNumbers",
    dqmDirectories = cms.vstring(
        '/harvested/data/ahMuTauAnalyzerOS_woBtag/FilterStatistics/evtSelDiTauCandidateForAHtoMuTauZeroCharge/events_passed_cumulative/',
        '/harvested/data/ahMuTauAnalyzerOS_wBtag/FilterStatistics/evtSelDiTauCandidateForAHtoMuTauZeroCharge/events_passed_cumulative/'
    ),
    runLumiSectionEventNumberFileName = cms.string(
        "/afs/cern.ch/user/v/veelken/public/selEvents_AHtoMuTau_2010Dec23.txt"
    ),
    separator = cms.string(':')                                                        
)
 
process.p = cms.Path(
    process.loadFilterStatistics
   + process.dumpDQMStore 
   + process.dumpRunLumiSectionEventNumbers
)
