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
    plots = cms.PSet(  
        dqmMonitorElements = cms.vstring(),
        processes = cms.vstring()
    ),
    title = cms.string(""),
    xAxis = cms.string('M'),
    yAxis = cms.string('numEntries_linear'),
    #yAxis = cms.string('numEntries_log'),
    legend = cms.string('regular'),
    labels = cms.vstring('mcNormScale'),                   
    drawOptionSet = cms.string('default'),
)

plotTemplateHist = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(

    ),

    xAxes = cms.PSet(
        M = copy.deepcopy(xAxis_mass)
    ),

    yAxes = cms.PSet(                         
        numEntries_linear = copy.deepcopy(yAxis_numEntries_linear),
        numEntries_log = copy.deepcopy(yAxis_numEntries_log)
    ),

    legends = cms.PSet(
        regular = cms.PSet(
            posX = cms.double(0.45),            
            posY = cms.double(0.69),             
            sizeX = cms.double(0.44),        
            sizeY = cms.double(0.20),            
            header = cms.string(''),          
            option = cms.string('brNDC'),       
            borderSize = cms.int32(0),          
            fillColor = cms.int32(0)             
        )
    ),

    labels = cms.PSet(
        mcNormScale = copy.deepcopy(label_mcNormScale)
    ),

    drawOptionSets = cms.PSet(
        default = cms.PSet(

        )
    ),

    drawJobs = cms.PSet(

    ),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsTemplateHist.ps')
    indOutputFileName = cms.string('plotTemplateHist_#PLOT#.png')
)
