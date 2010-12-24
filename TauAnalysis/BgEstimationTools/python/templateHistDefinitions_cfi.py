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
from TauAnalysis.BgEstimationTools.tools.drawTemplateHistConfigurator import drawTemplateHistConfigurator
from TauAnalysis.Configuration.makePlots2_grid import dqmHistPlotter_template
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.plotZtoMuTau_drawJobs_cfi import plots_ZtoMuTau

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

#--------------------------------------------------------------------------------
# define DQMHistPlotter module for plots comparing template shapes
#--------------------------------------------------------------------------------

plotHistZtoMuTau = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
        bgEstData = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = plotBgEstData.legendEntry,
            type = cms.string('Data')
        ),
        bgEstMC_pure = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = plotBgEstMC_pure.legendEntry,
            type = cms.string('smMC')
        ),
        bgEstMC_smSum = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = plotBgEstMC_smSum.legendEntry,
            type = cms.string('smSumMC')
        ),
        analysis = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = plotAnalysisMC_pure.legendEntry,
            type = cms.string('smMC')
        ),
        analysisZmumuEmbedding = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = plotAnalysis_ZmumuEmbedding.legendEntry,
            type = cms.string('smMC')
        )
    ),
                                                  
    xAxes = cms.PSet(
        Mass = copy.deepcopy(xAxis_mass)
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

    drawOptionEntries = cms.PSet(
        bgEstData = copy.deepcopy(drawOption_black_eff),
        bgEstMC_pure = copy.deepcopy(drawOption_green_eff),
        bgEstMC_smSum = copy.deepcopy(drawOption_lightBlue_eff),
        analysis = copy.deepcopy(drawOption_red_eff),
        analysisZmumuEmbedding = copy.deepcopy(drawOption_darkBlue_eff)
    ),

    drawJobs = cms.PSet(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    indOutputFileName = cms.string('')
)

#--------------------------------------------------------------------------------
# define auxiliary function for configuring control plots of cumulative distributions
#--------------------------------------------------------------------------------

def configurePlotZtoMuTauIntegrated(meName_data, meName_mc, meName_compatibility, 
	                            plotName, plotTitle,
                                    outputFileName):
    drawTemplateHistConfiguratorZtoMuTauIntegrated = drawTemplateHistConfigurator(
        template = drawJobTemplateHistIntegrated.clone(
            labels = cms.vstring('ksProb')
        )
    )    
    drawTemplateHistConfiguratorZtoMuTauIntegrated.add(
        meNames = [ meName_data, meName_mc ],
        name = plotName,
        title = plotTitle
    )
    plotHistZtoMuTauIntegrated = plotHistZtoMuTau.clone(
        labels = cms.PSet(
            ksProb = label_mcNormScale.clone(
                text = cms.vstring('KS prob.: %f1.2'),
                meName = cms.string(meName_compatibility)
            )
        ),
        drawJobs = drawTemplateHistConfiguratorZtoMuTauIntegrated.configure(),
        indOutputFileName = cms.string(outputFileName)
    )

    return plotHistZtoMuTauIntegrated

#--------------------------------------------------------------------------------
# define DQMHistPlotter module for (stacked) control plots 
# of various distributions in background enriched regions
#--------------------------------------------------------------------------------

plotZtoMuTauStacked_template = plots_ZtoMuTau.clone(
    plots = cms.PSet(
        dqmMonitorElements = cms.vstring(''),
        processes = cms.vstring(
            'TTplusJets',
            'diBoson',
            'Zmumu',
            'WplusJets',
            'QCD',
            'Ztautau',
            'Data'
        )
    ),
    stack = cms.vstring(
        'TTplusJets',
        'diBoson',
	'Zmumu',
        'WplusJets',
        'QCD',
        'Ztautau'
    )
)

plotHistZtoMuTauStacked = dqmHistPlotter_template.clone(
    processes = cms.PSet(
        Ztautau = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['ZtautauSum']['legendEntry']),
            type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['ZtautauSum']['type'])
        ),
        Zmumu = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['Zmumu_powheg']['legendEntry']),
            type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['Zmumu_powheg']['type'])
        ),
        WplusJets = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['WplusJets_madgraph']['legendEntry']),
            type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['WplusJets_madgraph']['type'])
        ),
        TTplusJets = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['TTplusJets_madgraph']['legendEntry']),
            type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['TTplusJets_madgraph']['type'])
        ),
        diBoson = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['VVsum']['legendEntry']),
            type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['VVsum']['type'])
        ),
        QCD = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['qcdSum']['legendEntry']),
            type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['qcdSum']['type'])
        ),
        Data = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['data']['legendEntry']),
            type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['data']['type'])
        )
    ),
    legends = cms.PSet(
        regular = legend_regular.clone(
            posX = cms.double(0.66),
            posY = cms.double(0.62),
            sizeX = cms.double(0.23),
            sizeY = cms.double(0.27)
        )
    ),
    labels = cms.PSet(
        mcNormScale = dqmHistPlotter_template.labels.mcNormScale.clone(
            posX = cms.double(0.17),
            posY = cms.double(0.74),
            sizeX = cms.double(0.16),
            sizeY = cms.double(0.15),
            textSize = cms.double(0.035),
            textAlign = cms.int32(12),
            text = cms.vstring(
                'CMS Preliminary', 
                'L = 36.2pb^{-1}',
                '#sqrt{s}=7TeV'
            )
        )
    ),
    drawOptionSets = cms.PSet(
        default = cms.PSet(
            Ztautau = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['ZtautauSum']['drawOption'],
            Zmumu = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['Zmumu_powheg']['drawOption'],
            WplusJets = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['WplusJets_madgraph']['drawOption'],
            TTplusJets = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['TTplusJets_madgraph']['drawOption'],
            diBoson = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['VVsum']['drawOption'],
            QCD = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['qcdSum']['drawOption'],
            Data = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['data']['drawOption']
        )
    ),
    drawJobs = cms.PSet(),
    outputFilePath = cms.string('./plots/'),
    indOutputFileName = cms.string(''),
)
