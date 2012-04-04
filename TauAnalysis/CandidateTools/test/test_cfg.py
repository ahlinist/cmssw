import FWCore.ParameterSet.Config as cms

#process = cms.Process("TEST")

process = cms.PSet(
    svFitCalibrator = cms.PSet(
    inputFileName     = cms.string("svFitPerformanceAnalysisPlots_all_2012Mar13.root"),
    outputFileName    = cms.string("testsvFitCalibrator.root"),
    outputTxtFileName = cms.string("testsvFitCalibrator.txt"),
    directories       = cms.vstring("DQMData/ZplusJets","DQMData/vbfHiggs120",),
    massPoints        = cms.vdouble(90,120,),
    histograms        = cms.vstring("muTau/nSVfitAnalyzerOption13/svFitMassVsSigmaDown","muTau/nSVfitAnalyzerOption15/svFitMassVsSigmaDown"),
    )
    )
