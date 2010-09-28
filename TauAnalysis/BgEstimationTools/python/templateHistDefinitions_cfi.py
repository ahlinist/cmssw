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
			meName = cms.string("diTauMvis12"),
		)
    ),
                                                          
    branchNamesEventWeight = cms.vstring('eventWeight'),

    norm = cms.double(1.)
)

#--------------------------------------------------------------------------------
# define drawing options used when making control plots of distributions
# in background enriched sample/event sample passing final selection criteria of the analysis
# --------------------------------------------------------------------------------

drawJobTemplateHist = cms.PSet(
    plots = cms.VPSet(
        cms.PSet(
            dqmMonitorElements = cms.vstring(''),
            process = cms.string('bgEstData'),
            drawOptionEntry = cms.string('bgEstData'),
            legendEntry = cms.string('Bg. enriched Data')
        ),
        cms.PSet(
            dqmMonitorElements = cms.vstring(''),
            process = cms.string('bgEstPure'),
            drawOptionEntry = cms.string('bgEstPure'),
            legendEntry = cms.string('pure Bg.')
        ),
        cms.PSet(
            dqmMonitorElements = cms.vstring(''),
            process = cms.string('finalEvtSel'),
            drawOptionEntry = cms.string('finalEvtSel'),
            legendEntry = cms.string('final Evt. Sel.')
        )
    ),
    norm = cms.double(1.),
    title = cms.string('M_{vis} (Muon + Tau)'),
    xAxis = cms.string('M'),
    yAxis = cms.string('numEntries_linear'),
    legend = cms.string('regular')
)

