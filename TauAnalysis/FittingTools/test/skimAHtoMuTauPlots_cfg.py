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
            ##'/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_all.root',
            '/data1/friis/Run28/plotsAHtoMuTau_all_bsmonly.root',
            '/data1/friis/Run28/plotsAHtoMuTau_all_smonly.root'
        ),
        dqmDirectory_store = cms.string('/')
    )
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.saveAnalysisResults = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_skimmed.root'),
    outputCommands = cms.vstring(
        'drop harvested/*',
        'keep harvested/*/ahMuTauAnalyzer_woBtag/afterEvtSelNonCentralJetEt20bTag/*',
        'keep harvested/*/ahMuTauAnalyzer_woBtag/FilterStatistics/*'
    )                                   
)

process.p = cms.Path(
    process.loadAnalysisResults
  #+ process.dumpDQMStore 
   + process.saveAnalysisResults
)
