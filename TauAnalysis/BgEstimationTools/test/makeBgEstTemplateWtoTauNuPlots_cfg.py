import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *
from TauAnalysis.BgEstimationTools.templateHistDefinitions_cfi import drawJobTemplateHist, drawJobTemplateHistIntegrated, drawJobAnalysisHistData, drawJobAnalysisHistMC, plotBgEstData, plotBgEstMC_pure, plotBgEstMC_smSum, plotAnalysisMC_pure, plotHistWtoTauNu, configurePlotWtoTauNuIntegrated, plotWtoTauNuStacked_template, plotHistWtoTauNuStacked
from TauAnalysis.BgEstimationTools.tools.drawTemplateHistConfigurator import drawTemplateHistConfigurator
from TauAnalysis.DQMTools.tools.drawJobConfigurator import *
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, getJobId

process = cms.Process('makeBgEstTemplateWtoTauNuPlots')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(0)
        )

process.source = cms.Source("EmptySource")


#--------------------------------------------------------------------
# initialize plotting parameters
#---------------------------------------------------------------
dqmDirectories_processes = {
    'Wtaunu' : {
        'template' : 'Wtaunu',
        'analysis' : 'Wtaunu'
        },
    'Wenu' : {
        'template' : 'Wenu',
        'analysis' : 'Wenu'
        },
    'Wmunu' : {
        'template' : 'Wmunu',
        'analysis' : 'Wmunu'
        },
    'Ztautau' : {
        'template' : 'Ztautau',
        'analysis' : 'Ztautau'
        },
    'QCD' : {
        'template' : 'QCD',
        'analysis' : 'QCD'
        },
    'Data' : {
        'template' : 'data',
        'analysis' : 'data',#_BTau_Run2010B_Dec22ReReco'
        },
    'smSum' : {
        'template' : 'smSum',
        'analysis' : 'smSum'
        },
    'ewkSum' : {
        'template' : 'ewkSum',
        'analysis' : 'smSum'
        }
    }

dqmDirectory_analysis = 'wTauNuAnalyzer/afterEvtSelHtRatio_beforeEvtSelMetTopology/'

dqmDirectories_control = {
    'MCTemplates'  : dqmDirectory_analysis,
    'QCDEnriched'  : 'BgEstTemplateAnalyzer_QCDEnriched/afterSingleTauCutBgEstQCDEnriched/'
}


plotTitles = {
    'Mt'      : "m_{T}(#tau,#nu)",
    'MtSVfit' : "m_{T}(#tau,#nu), SVfit method", 
    'pfMEt'   : "Particle-flow MET",
    'tauPt'   : "Tau--jet P_{T}",
    'tauEta'  : "Tau--jet #eta",
    'htRatio' : "HT-ratio",
    'metTopology' : "MET-topology"
}

xAxes = {
    'tauPt'   : 'Pt',
    'tauEta'  : 'Eta',
    'pfMEt'   : 'Pt',
    'Mt'      : 'Mt',
    'MtSVfit' : 'Mt',
    'htRatio' : 'unlabeled',
    'metTopology' : 'unlabeled'
}

rebinning_control = {
    'MCTemplates' : {
#        'Mt'          : 3,
#        'MtSVfit'     : 3,
        'metTopology' : 6
    },
    'QCDEnriched' : {
#        'Mt'          : 3,
 #       'MtSVfit'     : 3,
        'metTopology' : 6
        }
    }


pureProcessBgEnrichedSelections = {
    'QCDEnriched' : 'QCD'
}

meNames = {
    'Mt'           : 'TauNuCandidateQuantities/NuTauCandidateMt',
    'MtSVfit'      : 'TauNuCandidateQuantities/NuTauCandidateMtSVfit',
    'pfMEt'        : 'PFMEtQuantities/MEtPt',
    'tauPt'        : 'TauQuantities/TauPt',
    'tauEta'       : 'TauQuantities/TauEta',
    'htRatio'      : 'HtRatioQuantities/Ratio',
    'metTopology'  : 'MEtTopologyQuantities/Vratio'
}


#--------------------------------------------------------------------------------
# build dictionary, mapping
#  [process][selection]['template'/'analysis'][distName][distType] --> meName
#--------------------------------------------------------------------------------

meNameMapping = {}

for processName in dqmDirectories_processes.keys():
    meNameMapping[processName] = {}
    for selectionName in dqmDirectories_control.keys():
        meNameMapping[processName][selectionName] = {}
        for selectionType in [ "analysis", "template" ]:
            if selectionName == 'MCTemplates' and selectionType != 'analysis' :
                continue
            meNameMapping[processName][selectionName][selectionType] = {}
            for distName in meNames.keys(): 
                # check if histogram exists;
                # CV: a histogram is assumed to exist it has an entry in the rebinning options dictionary
                if rebinning_control[selectionName].get(distName) is not None:
                    meNameMapping[processName][selectionName][selectionType][distName] = {}
                    meName_distribution = None
                    if selectionType == 'analysis':
                        meName_distribution = \
                          '/analysis/harvested/' + dqmDirectories_processes[processName]['analysis'] \
                         + '/' + dqmDirectory_analysis + meNames[distName]
                    else:
                        meName_distribution = \
			  '/template/harvested/' + dqmDirectories_processes[processName]['template'] \
                         + '/' + dqmDirectories_control[selectionName] + meNames[distName]
                    meNameMapping[processName][selectionName][selectionType][distName]['distribution'] = meName_distribution
           
for processName, meNameMapping_process in meNameMapping.items():
    for selectionName, meNameMapping_selectionName in meNameMapping_process.items():
        for selectionType, meNameMapping_selectionType in meNameMapping_selectionName.items():
            for distName, meNameMapping_distName in meNameMapping_selectionType.items():
                meName_distribution = meNameMapping_distName['distribution']
                meNameMapping_distName['rebinned']   = meName_distribution + "Rebinned_%s" % selectionName
                meNameMapping_distName['template']   = meName_distribution + "Shape_%s" % selectionName
                meNameMapping_distName['integrated'] = meName_distribution + "IntegratedShape_%s" % selectionName

print("meNameMapping:")
print(meNameMapping)

#--------------------------------------------------------------------------------
# load template histogram of for different types of background events,
# obtained from background enriched control samples + analysis histograms
#--------------------------------------------------------------------------------

process.loadTemplateHistWtoTauNu = cms.EDAnalyzer("DQMFileLoader",
                                                  templateHist = cms.PSet(
    inputFileNames = cms.vstring(
    '/afs/cern.ch/user/l/liis/bgEst_2010_ht065_MET35/plotsWtoTauNu_bgEstTemplate_all.root'
    ),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('/template')
    )
)

process.loadAnalysisHistWtoTauNu = cms.EDAnalyzer("DQMFileLoader",
                                                  analysisHist = cms.PSet(
    inputFileNames = cms.vstring(
    '/afs/cern.ch/user/l/liis/Run07/plotsWtoTauNu_all.root'
    ),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('/analysis')
    )
                                                  )

#------------------------------------------------------------------------
# rebin in order to mitigate effect of small event statistics
#----------------------------------------------------------------------

process.rebinHistWtoTauNu = cms.Sequence()

jobsHistRebinning = {} # keys = [selectionName][distName]

for processName, meNameMapping_process in meNameMapping.items():
    for selectionName, meNameMapping_selectionName in meNameMapping_process.items():
        if jobsHistRebinning.get(selectionName) is None:
	    jobsHistRebinning[selectionName] = {}
        for selectionType, meNameMapping_selectionType in meNameMapping_selectionName.items():
            for distName, meNameMapping_distName in meNameMapping_selectionType.items():
                if jobsHistRebinning[selectionName].get(distName) is None:
	            jobsHistRebinning[selectionName][distName] = {}
                    jobsHistRebinning[selectionName][distName]['config'] = []
                    jobsHistRebinning[selectionName][distName]['binning'] = cms.PSet(
                        x = cms.PSet(
                           combineBins = cms.uint32(rebinning_control[selectionName][distName])
                        )
                    )
                jobsHistRebinning[selectionName][distName]['config'].append(cms.PSet(
                    meName_original = cms.string(meNameMapping_distName['distribution']),
                    meName_rebinned = cms.string(meNameMapping_distName['rebinned'])
                ))

for selectionName, jobsHistRebinning_selectionName in jobsHistRebinning.items():
    for distName, jobsHistRebinning_distName in jobsHistRebinning_selectionName.items():
        rebinningModule = cms.EDAnalyzer("DQMHistRebinner",
	    config = cms.VPSet(jobsHistRebinning[selectionName][distName]['config']),
            binning = jobsHistRebinning[selectionName][distName]['binning']
        )
	rebinningModuleName = "rebinTemplateHistWtoTauNu%s%s" % (selectionName, distName)
        setattr(process, rebinningModuleName, rebinningModule)

        process.rebinHistWtoTauNu += rebinningModule


#--------------------------------------------------------------------------------
# normalize to unit area distribution of Mt(tau,nu) for W --> tau nu signal
# and different types of background events
#--------------------------------------------------------------------------------

jobsHistNormalization = []

for processName, meNameMapping_process in meNameMapping.items():
    for selectionName, meNameMapping_selectionName in meNameMapping_process.items():
        for selectionType, meNameMapping_selectionType in meNameMapping_selectionName.items():
            for distName, meNameMapping_distName in meNameMapping_selectionType.items():
                jobsHistNormalization.append(cms.PSet(
                    meName_input = cms.string(meNameMapping_distName['rebinned']),
                    meName_output = cms.string(meNameMapping_distName['template'])
                ))

process.normalizeHistWtoTauNu = cms.EDAnalyzer("DQMHistNormalizer",
    config = cms.VPSet(jobsHistNormalization),
    norm = cms.double(1.)
)

#--------------------------------------------------------------------------------
# produce cumulative distributions
#--------------------------------------------------------------------------------

jobsHistIntegration = []

for processName, meNameMapping_process in meNameMapping.items():
    for selectionName, meNameMapping_selectionName in meNameMapping_process.items():
        for selectionType, meNameMapping_selectionType in meNameMapping_selectionName.items():
            for distName, meNameMapping_distName in meNameMapping_selectionType.items():
	        jobsHistIntegration.append(cms.PSet(
                    meName_input = cms.string(meNameMapping_distName['template']),
                    meName_output = cms.string(meNameMapping_distName['integrated']),
                    integrateFrom = cms.string("right")
                ))
    
process.integrateHistWtoTauNu = cms.EDAnalyzer("DQMHistIntegrator",
    config = cms.VPSet(jobsHistIntegration)
)

#--------------------------------------------------------------------------------
# compute Kolmogorov-Smirnov probabilities for distributions observed in Data
# to agree with Monte Carlo predictions in the background enriched regions
#--------------------------------------------------------------------------------

jobsKolmogorovTest = []

meNameMapping_mc = meNameMapping['smSum']
meNameMapping_data = meNameMapping['Data']
for selectionName, meNameMapping_selectionName_data in meNameMapping_data.items():
    selectionType = 'template'
    if meNameMapping_selectionName_data.get(selectionType) is not None:
        meNameMapping_selectionType_data = meNameMapping_selectionName_data[selectionType]
        for distName in [ "Mt", "MtSVfit"]:#, "pfMEt" ]:
            if meNameMapping_selectionType_data.get(distName) is not None:
                meNameMapping_distName_data = meNameMapping_selectionType_data[distName]
                   
                meName_distribution_data = meNameMapping_distName_data['distribution']
	        meName_compatibility = meName_distribution + "Compatibility"
                meNameMapping_distName_data['compatibility'] = meName_compatibility

                jobsKolmogorovTest.append(cms.PSet(
                    meName_test = cms.string(meNameMapping_distName_data['template']),
                    meName_reference = cms.string(meNameMapping_mc[selectionName][selectionType][distName]['template']),
                    meName_compatibility = cms.string(meName_compatibility)
    	        ))
    
process.compKolmogorovProbWtoTauNu = cms.EDAnalyzer("DQMHistKolmogorovTest",
    config = cms.VPSet(jobsKolmogorovTest)
)

#--------------------------------------------------------------------------------
# plot template histograms obtained from Monte Carlo
# compared to the shapes determined by background enriched regions in (pseudo)Data
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorWtoTauNu = drawTemplateHistConfigurator(
    template = drawJobTemplateHist
)

drawAnalysisHistConfiguratorWtoTauNuData = drawTemplateHistConfigurator(
    template = drawJobAnalysisHistData
)

drawAnalysisHistConfiguratorWtoTauNuMC = drawTemplateHistConfigurator(
    template = drawJobAnalysisHistMC
)

#process.plotTemplateHistWtoTauNuIntegrated = cms.Sequence()

meNameMapping_mcSMsum = meNameMapping['smSum']
meNameMapping_data = meNameMapping['Data']
for selectionName, meNameMapping_selectionName_data in meNameMapping_data.items():
    if pureProcessBgEnrichedSelections.get(selectionName) is not None:
        meNameMapping_mcPureProcess = meNameMapping[pureProcessBgEnrichedSelections[selectionName]]
        selectionType = 'template'
        if meNameMapping_selectionName_data.get(selectionType) is not None:
            meNameMapping_selectionType_data = meNameMapping_selectionName_data[selectionType]
	    for distName, meNameMapping_distName_data in meNameMapping_selectionType_data.items():
                # configure histograms for MC(SM|bgEst) vs. MC(pure|bgEst) vs. Data|bgEst comparisson
                # (showing Data to Monte Carlo comparisson and effect of impurities in background enriched control regions)
                drawTemplateHistConfiguratorWtoTauNu.add(
                    meNames = [
                        meNameMapping_mcSMsum[selectionName][selectionType][distName]['template'],
                        meNameMapping_mcPureProcess[selectionName][selectionType][distName]['template'],
                        meNameMapping_distName_data['template']
                    ],
                    name = ("%s_%s" % (selectionName, distName)),
                    title = ("%s: %s" % (selectionName, plotTitles[distName]))
                )
#
#                if meNameMapping_distName_data.get('compatibility') is not None:
#                    plotHistWtoTauNuIntegrated = configurePlotWtoTauNuIntegrated(
#    	                meNameMapping_distName_data['template'],
#    	                meNameMapping_mcSMsum[selectionName][selectionType][distName]['template'],
#                        meNameMapping_distName_data['compatibility'],
#                        "%s_%s" % (selectionName, distName),
#	                "%s: %s" % (selectionName, plotTitles[distName]),
#	                "plotBgEstTemplateWtoTauNuIntegrated_#PLOT#.pdf"
#                    )
#                    plotHistWtoTauNuIntegratedName = "plotHistWtoTauNuIntegrated%s%s" % (selectionName, distName)
#                    setattr(process, plotHistWtoTauNuIntegratedName, plotHistWtoTauNuIntegrated)
#	            process.plotTemplateHistWtoTauNuIntegrated += plotHistWtoTauNuIntegrated

                # configure histograms for Data|bgEst vs. MC(pure|analysis) comparisson
                # (showing possible bias on mass shape caused by event selection criteria applied in analysis)
                drawAnalysisHistConfiguratorWtoTauNuData.add(
                    meNames = [
                        meNameMapping_distName_data['template'],
                        meNameMapping_mcPureProcess[selectionName]['analysis'][distName]['template']
                    ],
                    name = ("%s_%s" % (selectionName, distName)),
                    title = ("%s: %s" % (selectionName, plotTitles[distName]))
                )
                
#--------------------------------------------------------------------------
# configure histograms for MC(SM|bgEst) vs. MC(pure|bgEst)
# (showing effect of impurities in background enriched control regions)
#-------------------------------------------------------------------------

                drawAnalysisHistConfiguratorWtoTauNuMC.add(
                    meNames = [
                        meNameMapping_mcSMsum[selectionName][selectionType][distName]['template'],
                        meNameMapping_mcPureProcess[selectionName]['analysis'][distName]['template']
                    ],
                    name = ("%s_%s" % (selectionName, distName)),
                    title = ("%s: %s" % (selectionName, plotTitles[distName]))
                )

process.plotTemplateHistWtoTauNu = plotHistWtoTauNu.clone(
    drawJobs = drawTemplateHistConfiguratorWtoTauNu.configure(),
    indOutputFileName = cms.string('plotBgEstTemplateWtoTauNu_#PLOT#.pdf')
    )

process.plotAnalysisHistWtoTauNuData = plotHistWtoTauNu.clone(
    drawJobs = drawAnalysisHistConfiguratorWtoTauNuData.configure(),
    indOutputFileName = cms.string('plotBgEstTemplateData_vs_AnalysisWtoTauNu_#PLOT#.pdf')
)

process.plotAnalysisHistWtoTauNuMC = plotHistWtoTauNu.clone(
    drawJobs = drawAnalysisHistConfiguratorWtoTauNuMC.configure(),
    indOutputFileName = cms.string('plotBgEstTemplateMC_vs_AnalysisWtoTauNu_#PLOT#.pdf')
)

process.plotAnalysisHistWtoTauNuStacked = cms.Sequence()

meNameMapping_data = meNameMapping['Data']
for selectionName, meNameMapping_selectionName_data in meNameMapping_data.items():
    selectionType = 'template'
    if meNameMapping_selectionName_data.get(selectionType) is not None:
        meNameMapping_selectionType_data = meNameMapping_selectionName_data[selectionType]

        drawJobConfiguratorWtoTauNuStacked = drawJobConfigurator(
            template = plotWtoTauNuStacked_template,
            dqmDirectory = '#PROCESSDIR#' + '/' + dqmDirectories_control[selectionName]
            )

plots = [ ]

for distName in meNameMapping_selectionType_data.keys():
    plots.append(drawJobConfigEntry(
            meName = meNames[distName] + "Rebinned_%s" % selectionName,
            title = plotTitles[distName],
            xAxis = xAxes[distName],
            name = ("%s" % distName)
            ))

drawJobConfiguratorWtoTauNuStacked.add(plots = plots)

dqmHistPlotterModule = plotHistWtoTauNuStacked.clone(
    processes = cms.PSet(
        Ztautau = plotHistWtoTauNuStacked.processes.Ztautau.clone(
            dqmDirectory = cms.string('/template/harvested/' + dqmDirectories_processes['Ztautau']['template'])
            ),
        Wmunu = plotHistWtoTauNuStacked.processes.Wmunu.clone(
            dqmDirectory = cms.string('/template/harvested/' + dqmDirectories_processes['Wmunu']['template'])
            ),
        Wenu = plotHistWtoTauNuStacked.processes.Wenu.clone(
            dqmDirectory = cms.string('/template/harvested/' + dqmDirectories_processes['Wenu']['template'])
            ),
        Wtaunu = plotHistWtoTauNuStacked.processes.Wtaunu.clone(
            dqmDirectory = cms.string('/template/harvested/' + dqmDirectories_processes['Wtaunu']['template'])
            ),
        QCD = plotHistWtoTauNuStacked.processes.QCD.clone(
            dqmDirectory = cms.string('/template/harvested/' + dqmDirectories_processes['QCD']['template'])
            ),
        Data = plotHistWtoTauNuStacked.processes.Data.clone(
            dqmDirectory = cms.string('/template/harvested/' + dqmDirectories_processes['Data']['template'])
            )
        ),
    drawJobs = drawJobConfiguratorWtoTauNuStacked.configure(),
    indOutputFileName = cms.string('bgEstControlWtoTauNu_%s_#PLOT#.pdf' % selectionName)
    )

dqmHistPlotterModuleName = "plotWtoTauNuStacked%s" % selectionName
setattr(process, dqmHistPlotterModuleName, dqmHistPlotterModule)

process.plotAnalysisHistWtoTauNuStacked += dqmHistPlotterModule


process.saveBgEstTemplateHistWtoTauNu = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string(
         'bgEstTemplateHistWtoTauNu.root'
    ),
    outputCommands = cms.vstring(
        'drop *',
        'keep /template/harvested/*/BgEstTemplateAnalyzer_QCDEnriched/afterSingleTauCutBgEstQCDEnriched/MEtTopologyQuantities',
        'keep /analysis/harvested/*/wTauNuAnalyzer/afterEvtSelHtRatio_beforeEvtSelMetTopology/MEtTopologyQuantities/*'
        
    )
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.p = cms.Path(
    process.loadTemplateHistWtoTauNu
    +process.loadAnalysisHistWtoTauNu
    +process.rebinHistWtoTauNu
    +process.normalizeHistWtoTauNu
#    +process.integrateHistWtoTauNu
#    +process.compKolmogorovProbWtoTauNu    
##    +process.dumpDQMStore
#    +process.plotTemplateHistWtoTauNu
#    +process.plotTemplateHistWtoTauNuIntegrated
#    +process.plotAnalysisHistWtoTauNuData
#    +process.plotAnalysisHistWtoTauNuMC
#    +process.plotAnalysisHistWtoTauNuStacked
    +process.saveBgEstTemplateHistWtoTauNu
    )
