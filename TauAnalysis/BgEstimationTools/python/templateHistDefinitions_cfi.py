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

prodTemplateHist = cms.EDAnalyzer("TemplateHistProducer",
    fileNames = cms.vstring(),
    treeName = cms.string("ntupleProducer/bgEstEvents"),

    treeSelection = cms.string(""),   
                                                          
    branchName = cms.string('diTauMvis12'),
    #branchName = cms.string('diTauMt1MET'),               
    branchNameEventWeight = cms.string('eventWeight'),

    meName = cms.string(""),
    numBinsX = cms.uint32(30),
    xMin = cms.double(0.),
    xMax = cms.double(150.)
    #numBinsX = cms.uint32(11),                                                      
    #xBins = cms.vdouble( 0., 20., 30., 40., 50., 60., 70., 80., 90., 100., 120., 150. )                                        
)

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
    title = cms.string('M_{vis}(Muon + Tau)'),
    xAxis = cms.string('M'),
    yAxis = cms.string('numEntries_linear'),
    legend = cms.string('regular'),
    labels = cms.vstring('')
)

