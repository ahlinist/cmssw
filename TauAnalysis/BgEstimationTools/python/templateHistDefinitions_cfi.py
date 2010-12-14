import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Define modules for filling and plotting histograms
# used by "template" method for data-driven background estimation
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

#--------------------------------------------------------------------------------
# define observable to be used as template
# and histogram binning options
#
# NOTE: binning needs to match that of the final analysis,
#       defined in TauAnalysis/Core/plugins/..HistManager.cc
# --------------------------------------------------------------------------------

prodTemplateHist = cms.EDAnalyzer("TemplateHistProducer",
    fileNames = cms.vstring(),
    treeName = cms.string("ntupleProducer/bgEstEvents"),

    treeSelection = cms.string(""),

    config = cms.VPSet(
        cms.PSet(
	    variables = cms.VPSet(
                cms.PSet(
                    branchName = cms.string('selDiTauMvis12_0'),
                    numBins = cms.uint32(40),
                    min = cms.double(0.),
                    max = cms.double(200.)
                )
            ),
            meName = cms.string("diTauMvis12")
        )
    ),
                                                          
    branchNamesEventWeight = cms.vstring('eventWeight'),

    norm = cms.double(1.)
)

#--------------------------------------------------------------------------------
# define drawing options used when making control plots of distributions
# in background enriched sample/event sample passing final selection criteria of the analysis
# --------------------------------------------------------------------------------

plotBgEstData = cms.PSet(
    dqmMonitorElements = cms.vstring(''),
    process = cms.string('bgEstData'),
    drawOptionEntry = cms.string('bgEstData'),
    legendEntry = cms.string('Bg. enriched Data')
)
plotBgEstMC_pure = cms.PSet(
    dqmMonitorElements = cms.vstring(''),
    process = cms.string('bgEstMC_pure'),
    drawOptionEntry = cms.string('bgEstMC_pure'),
    legendEntry = cms.string('pure Bg. MC')
)
plotBgEstMC_smSum = cms.PSet(
    dqmMonitorElements = cms.vstring(''),
    process = cms.string('bgEstMC_smSum'),
    drawOptionEntry = cms.string('bgEstMC_smSum'),
    legendEntry = cms.string('Bg. enriched MC')
)
plotAnalysisMC_pure = cms.PSet(
    dqmMonitorElements = cms.vstring(''),
    process = cms.string('analysis'),
    drawOptionEntry = cms.string('analysis'),
    legendEntry = cms.string('Z #rightarrow #mu + #tau-jet Analysis')
)
plotAnalysis_ZmumuEmbedding = cms.PSet(
    dqmMonitorElements = cms.vstring(''),
    process = cms.string('analysisZmumuEmbedding'),
    drawOptionEntry = cms.string('analysisZmumuEmbedding'),
    legendEntry = cms.string('Z #rightarrow #mu + #mu Embedding')
)

drawJobTemplateHist = cms.PSet(
    plots = cms.VPSet(
        plotBgEstMC_smSum,
        plotBgEstMC_pure,
        plotBgEstData
    ),
    norm = cms.double(1.),
    title = cms.string(''),
    xAxis = cms.string('Mass'),
    yAxis = cms.string('numEntries_linear'),
    legend = cms.string('regular')
)

drawJobTemplateHistIntegrated = cms.PSet(
    plots = cms.VPSet(
        plotBgEstMC_smSum,
        plotBgEstData
    ),
    norm = cms.double(1.),
    title = cms.string(''),
    xAxis = cms.string('Mass'),
    yAxis = cms.string('numEntries_linear'),
    legend = cms.string('regular'),
    labels = cms.vstring()
)

drawJobAnalysisHistData = cms.PSet(
    plots = cms.VPSet(
        plotBgEstData,
        plotAnalysisMC_pure
    ),
    norm = cms.double(1.),
    title = cms.string(''),
    xAxis = cms.string('Mass'),
    yAxis = cms.string('numEntries_linear'),
    legend = cms.string('regular')
)

drawJobAnalysisHistMC = cms.PSet(
    plots = cms.VPSet(
        plotBgEstMC_smSum,
        plotAnalysisMC_pure
    ),
    norm = cms.double(1.),
    title = cms.string(''),
    xAxis = cms.string('Mass'),
    yAxis = cms.string('numEntries_linear'),
    legend = cms.string('regular')
)

drawJobAnalysisHistZmumuEmbedding = cms.PSet(
    plots = cms.VPSet(
        plotAnalysisMC_pure,
        plotAnalysis_ZmumuEmbedding
    ),
    norm = cms.double(1.),
    title = cms.string(''),
    xAxis = cms.string('Mass'),
    yAxis = cms.string('numEntries_linear'),
    legend = cms.string('regular')
)
