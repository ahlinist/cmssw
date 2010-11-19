import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi import *

#--------------------------------------------------------------------------------
# Drop MonitorElements not needed for exporting analysis results into ASCII files,
# in order to reduce memory consumption
#--------------------------------------------------------------------------------

process = cms.Process('skimAHtoElecTauPlots')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.loadAnalysisResults = cms.EDAnalyzer("DQMFileLoader",
    all = cms.PSet(
        inputFileNames = cms.vstring(
            '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsZtoElecTau_all.root'
        ),
        dqmDirectory_store = cms.string('/')
    )
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.saveAnalysisResults = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsZtoElecTau_skimmed.root'),
    outputCommands = cms.vstring(
        'drop /summed/harvested/*',
        'keep /summed/harvested/Ztautau/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/*',
        'keep /summed/harvested/Ztautau/zElecTauAnalyzer/FilterStatistics/*',
        'keep /summed/harvested/Zee/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/*',
        'keep /summed/harvested/Zee/zElecTauAnalyzer/FilterStatistics/*',        
        'keep /summed/harvested/qcdBCtoESum/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/*',
        'keep /summed/harvested/qcdBCtoESum/zElecTauAnalyzer/FilterStatistics/*',
        'keep /summed/harvested/qcdEMenrichedSum/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/*',
        'keep /summed/harvested/qcdEMenrichedSum/zElecTauAnalyzer/FilterStatistics/*',
        'keep /summed/harvested/WtoENu/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/*',
        'keep /summed/harvested/WtoENu/zElecTauAnalyzer/FilterStatistics/*',
        'keep /summed/harvested/WtoTauNu/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/*',
        'keep /summed/harvested/WtoTauNu/zElecTauAnalyzer/FilterStatistics/*',
        'keep /summed/harvested/TTplusJets/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/*',
        'keep /summed/harvested/TTplusJets/zElecTauAnalyzer/FilterStatistics/*',
        'keep /summed/harvested/gammaPlusJetsSum/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/*',
        'keep /summed/harvested/gammaPlusJetsSum/zElecTauAnalyzer/FilterStatistics/*'
    )                                   
)

process.p = cms.Path(
    process.loadAnalysisResults
  #+ process.dumpDQMStore 
   + process.saveAnalysisResults
)
