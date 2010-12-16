import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi import *

#--------------------------------------------------------------------------------
# Drop MonitorElements not needed for exporting analysis results into ASCII files,
# in order to reduce memory consumption
#--------------------------------------------------------------------------------

process = cms.Process('skimAHtoMuTauPlots')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.loadAnalysisResults = cms.EDAnalyzer("DQMFileLoader",
    all = cms.PSet(
        inputFileNames = cms.vstring(
            #'/data2/squires/Run29/plotsAHtoMuTau_allHiggs.root',
            #'/data2/squires/Run29/plotsAHtoMuTau_allOthers.root',
            '/data2/friis/Run35SYS/plots_all.bsm.root',                                        
            '/data2/friis/Run35SYS/plots_all.sm.root'                                          
        ),
        dqmDirectory_store = cms.string('/')
    )
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.saveAnalysisResults = cms.EDAnalyzer("DQMSimpleFileSaver",
    #outputFileName = cms.string('/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCloose_woSys_skimmed.root'),
    outputFileName = cms.string('/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCmedium_wSys_skimmed.root'),
    outputCommands = cms.vstring(
        'drop harvested/*',
        'keep harvested/*/ahMuTauAnalyzerOS_woBtag/afterEvtSelDiTauCandidateForAHtoMuTauZeroCharge/*',
        'keep harvested/*/ahMuTauAnalyzerOS_woBtag/FilterStatistics/*',
        'keep harvested/*/ahMuTauAnalyzerOS_wBtag/afterEvtSelDiTauCandidateForAHtoMuTauZeroCharge/*',
        'keep harvested/*/ahMuTauAnalyzerOS_wBtag/FilterStatistics/*'
    )                                   
)

process.p = cms.Path(
    process.loadAnalysisResults
  #+ process.dumpDQMStore 
   + process.saveAnalysisResults
)
