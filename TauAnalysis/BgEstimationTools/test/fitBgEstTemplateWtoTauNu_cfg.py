import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Apply "template" method for data-driven background estimation
# to W --> tau-jet + nu channel
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *
from TauAnalysis.BgEstimationTools.templateHistDefinitions_cfi import \
       drawJobTemplateHist, drawJobAnalysisHistData, drawJobAnalysisHistMC, \
         plotBgEstData, plotBgEstMC_pure, plotBgEstMC_smSum, plotAnalysisMC_pure
from TauAnalysis.BgEstimationTools.tools.drawTemplateHistConfigurator import drawTemplateHistConfigurator
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, getJobId

process = cms.Process('fitBgEstTemplateWtoTauNu')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(0)
        )

process.source = cms.Source("EmptySource")

dqmDirectoriesProcess = {
    'ewkSum'     : 'ewkSum',
    'Ztautau'    : 'Ztautau',
    'Wmunu'      : 'Wmunu',
    'Wenu'       : 'Wenu',
    'Wtaunu'     : 'Wtaunu',
    'Data'       : 'data'
    }

dqmDirectoryAnalysis = 'wTauNuAnalyzer/afterEvtSelHtRatio_beforeEvtSelMetTopology/'

dqmDirectoriesBgEnrichedSelections = {
    'QCDEnriched'  : 'BgEstTemplateAnalyzer_QCDEnriched/afterSingleTauCutBgEstQCDEnriched/'
    }

pureProcessBgEnrichedSelections = {
    'QCDEnriched' : 'QCD'
    }

meNames = {
    'Mt'            : 'TauNuCandidateQuantities/NuTauCandidateMt',
    'MtSVfit'       : 'TauNuCandidateQuantities/NuTauCandidateMtSVfit',
    'MEtTopology'   : 'MEtTopologyQuantities/Vratio'
    }

#--------------------------------------------------------------------------------
# load template histogram for W --> tau nu signal and different types of background events,
# and distributions observed in (pseudo)data in events passing final analysis selection criteria
#--------------------------------------------------------------------------------

process.loadAnalysisResultsWtoTauNu = cms.EDAnalyzer("DQMFileLoader",
                                                     Wtaunu = cms.PSet(
        inputFileNames = cms.vstring(
            '/afs/cern.ch/user/l/liis/scratch0/CMSSW_3_9_7/src/TauAnalysis/BgEstimationTools/test/bgEstTemplateHistWtoTauNu.root'
            ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('/')
        )
                                                     )

process.subHist = cms.EDAnalyzer("DQMHistSubtractor",
                                 config = cms.VPSet(
    cms.PSet(
    meName_minuend = cms.string('/analysis/harvested/data/wTauNuAnalyzer/afterEvtSelHtRatio_beforeEvtSelMetTopology/MEtTopologyQuantities/VratioRebinned_QCDEnriched'),
    meName_subtrahend = cms.string('/analysis/harvested/smSum/wTauNuAnalyzer/afterEvtSelHtRatio_beforeEvtSelMetTopology/MEtTopologyQuantities/VratioRebinned_QCDEnriched'),
    meName_difference = cms.string('/template/harvested/data/BgEstTemplateAnalyzer_QCDEnriched/afterSingleTauCutBgEstQCDEnriched/MEtTopologyQuantities/VratoQCD')
    )
    )
                                 )

process.normHist = cms.EDAnalyzer("DQMHistNormalizer",
                                  config = cms.VPSet(
    cms.PSet(
    meName_input = cms.string('/template/harvested/data/BgEstTemplateAnalyzer_QCDEnriched/afterSingleTauCutBgEstQCDEnriched/MEtTopologyQuantities/VratoQCD'),
    meName_output = cms.string('/template/harvested/data/BgEstTemplateAnalyzer_QCDEnriched/afterSingleTauCutBgEstQCDEnriched/MEtTopologyQuantities/VratoQCDnorm')
    )
    ),
                                  norm = cms.double(1.)
                                  )


#--------------------------------------------------------------------------------
#  Landau distribution convoluted with Gaussian
#----------------------------------------------------------------------------
transMass_smoothing = cms.PSet(
    pluginName = cms.string("Landau_convoluted_with_Gaussian"),
    pluginType = cms.string("SmoothLandau_x_GaussPdfWrapper"), # defaults to SmoothGenericPdfWrapper
    xMin = cms.double(20.),
    xMax = cms.double(200.),
    excludeBinsX = cms.vdouble(),
    parameter = cms.PSet(
        width = cms.PSet( # width (scale) parameter of Landau density
            initial = cms.double(5.),
            min = cms.double(0.01),
            max = cms.double(50.)
        ),
        mp = cms.PSet( # most probable (MP, location) parameter of Landau density
            initial = cms.double(50.),
            min = cms.double(30.),
            max = cms.double(150.) 
        ),
        area = cms.PSet( # total area (integral from -inf to +inf, normalization constant)
            initial = cms.double(1.),
            min = cms.double(0.1),
            max = cms.double(10.)
        ),
        gsigma = cms.PSet( # width (sigma) of convoluted Gaussian function
            initial = cms.double(10.),
            min = cms.double(1.),
            max = cms.double(50.)
        )
    )
    )                                                    
                    
            
#-------------------------------------------------------------------
# fit
#-----------------------------------------------------------------

process.fitWtoTauNu = cms.EDAnalyzer("TemplateHistFitter",
                                     processes = cms.PSet(
        Wtaunu = cms.PSet(
            templates = cms.PSet(
                Mt = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/'+ '/' + dqmDirectoriesProcess['Wtaunu'] + '/' + dqmDirectoryAnalysis + meNames['Mt'] + 'Shape' + '_' + 'MCTemplates'
                        )
                    ),
                MEtTopology = cms.PSet(
                    meName = cms.string(
                       '/analysis/harvested'+ '/' + dqmDirectoriesProcess['Wtaunu'] + '/' + dqmDirectoryAnalysis + meNames['MEtTopology'] + 'Shape' + '_' + 'MCTemplates'
                       )
                    )
                ),
            norm = cms.PSet(
                initial = cms.double(175.)
                ),
            drawOptions = copy.deepcopy(drawOption_Wtaunu_separate)
            ),
        Ztautau = cms.PSet(
            templates = cms.PSet(
                Mt = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/' + dqmDirectoriesProcess['Ztautau'] + '/' + dqmDirectoryAnalysis + meNames['Mt'] + 'Shape' + '_' + 'MCTemplates'
                        )
                    ),
                MEtTopology = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/' + dqmDirectoriesProcess['Ztautau'] + '/' + dqmDirectoryAnalysis + meNames['MEtTopology'] + 'Shape' + '_' + 'MCTemplates'
                        )
                    )
                ),
            norm = cms.PSet(
                initial = cms.double(10.)
                ),
            drawOptions = copy.deepcopy(drawOption_Ztautau_separate)
            ),
        Wenu = cms.PSet(
            templates = cms.PSet(
                Mt = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/'+ '/' + dqmDirectoriesProcess['Wenu'] + '/' + dqmDirectoryAnalysis + meNames['Mt'] + 'Shape' + '_' + 'MCTemplates'
                        )
                    ),
                MEtTopology = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested'+ '/' + dqmDirectoriesProcess['Wenu'] + '/' + dqmDirectoryAnalysis + meNames['MEtTopology'] + 'Shape' + '_' + 'MCTemplates'
                       )
                    )
                ),
            norm = cms.PSet(
                initial = cms.double(4.)
                ),
            drawOptions = copy.deepcopy(drawOption_Wenu_separate)
            ),   
         Wmunu = cms.PSet(
            templates = cms.PSet(
                Mt = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/'+ '/' + dqmDirectoriesProcess['Wmunu'] + '/' + dqmDirectoryAnalysis + meNames['Mt'] + 'Shape' + '_' + 'MCTemplates'
                        )
                    ),
                MEtTopology = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested'+ '/' + dqmDirectoriesProcess['Wmunu'] + '/' + dqmDirectoryAnalysis + meNames['MEtTopology'] + 'Shape' + '_' + 'MCTemplates'
                       )
                    )
                ),
            norm = cms.PSet(
                initial = cms.double(4.)
                ),
            drawOptions = copy.deepcopy(drawOption_Wmunu_separate)
            ),   
        QCD = cms.PSet(
            templates = cms.PSet(
                Mt = cms.PSet( 
                    meName = cms.string(
                        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
                        + dqmDirectoriesBgEnrichedSelections['QCDEnriched'] \
                        + meNames['Mt'] + 'Shape' + '_' + 'QCDEnriched'
                        )
                    ),
#                MtSVfit = cms.PSet(
#                    meName = cms.string(
#                        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
#                            + dqmDirectoriesBgEnrichedSelections['QCDEnriched'] \
#                            + meNames['MtSVfit'] + 'Shape' + '_' + 'QCDEnriched'
#                        )
#                    ),
                MEtTopology = cms.PSet(
                    meName = cms.string(
    '/template/harvested/data/BgEstTemplateAnalyzer_QCDEnriched/afterSingleTauCutBgEstQCDEnriched/MEtTopologyQuantities/VratoQCDnorm'
#                        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
#                        + dqmDirectoriesBgEnrichedSelections['QCDEnriched'] \
#                        + meNames['MEtTopology'] + 'Shape' + '_' + 'QCDEnriched'
#                        + meNames['MEtTopology'] + 'QCD' + '_' + 'norm'
                        )
                    )
                ),
            norm = cms.PSet(
                initial = cms.double(100.)
                ),
            drawOptions = copy.deepcopy(drawOption_QCD_separate)
            )
        ),
 # use bg enriched data-samples consisting of all MC processes for testing
        data = cms.PSet(
            distributions = cms.PSet(
            Mt = cms.PSet(
                meName = cms.string(
                    '/analysis/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoryAnalysis \
                        + meNames['Mt'] + 'Rebinned' + '_' + 'MCTemplates'
                    )
                ),
            MtSVfit = cms.PSet(
                meName = cms.string(
                    '/analysis/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoryAnalysis \
                        + meNames['MtSVfit'] + 'Rebinned' + '_' + 'MCTemplates'
                    )
                ),
            MEtTopology = cms.PSet(
                meName = cms.string(
                    '/analysis/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoryAnalysis \
                        + meNames['MEtTopology'] + 'Rebinned' + '_' + 'MCTemplates'
                    )
                )
            )
            ),
                                     
    fit = cms.PSet(
        algorithm = cms.PSet(
            pluginName = cms.string("fitBgEstTemplateWtoTauNuAlgorithm"),
            pluginType = cms.string("TemplateFitAdapter_RooFit")
            ),
        variables = cms.PSet(
#            Mt = cms.PSet(
#                name = cms.string("Mt"),
#                title = cms.string("M_{T}(Tau + Nu)"),
#                min = cms.double(40.),
#                max = cms.double(200.)
#                )
#            MtSVfit = cms.PSet(
#                name = cms.string("MtSVfit"),
#                title = cms.string("M_{T}(Tau + Nu) SV-fit"),
#                min = cms.double(20.),
#                max = cms.double(200.)
#                )
            MEtTopology = cms.PSet(
                name = cms.string("MEtTopology"),
                title = cms.string("MET-topology"),
                min = cms.double(0.),
                max = cms.double(1.)
                )
            ),
        constraints = cms.PSet(
            Ztautau = cms.PSet(
                norm = cms.PSet(
                    value = cms.double(1.*10.),
                    uncertainty = cms.double(1.*10)
                    )
                ),
            Wenu = cms.PSet(
                norm = cms.PSet(
                    value = cms.double(1.*4.),
                    uncertainty = cms.double(1.*4.)
                    )                
                ),
            Wmunu = cms.PSet(
                norm = cms.PSet(
                    value = cms.double(1.*1.5),
                    uncertainty = cms.double(1.*1.5)
                    )
                )
            ),
        cutUnfittedRegion = cms.bool(False),
        verbosity = cms.PSet(
            printLevel = cms.int32(1),
            printWarnings = cms.bool(True)
            )
        ),
   estStatUncertainties = cms.PSet(
        numSamplings = cms.int32(0),
        chi2redMax = cms.double(10.),
        verbosity = cms.PSet(
            printLevel = cms.int32(-1),
            printWarnings = cms.bool(False)
        )
    ),

    estSysUncertainties = cms.PSet(
        fluctuations = cms.PSet(
            ),
        numSamplings = cms.int32(0),
        chi2redMax = cms.double(1000),
        verbosity = cms.PSet(
            printLevel = cms.int32(-1),
            printWarnings = cms.bool(False)
            )
        ),
     output = cms.PSet(
        controlPlots = cms.PSet(
            fileName = cms.string("./plots/fitBgEstTemplateWtoTauNu_#PLOT#.pdf")
            ),
        fitResults = cms.PSet(
            dqmDirectory = cms.string('/fitResults')
            )
        )
                                     )
#-----------------------------------------------
# save output
#-------------------------------------------------
process.saveFitResultsWtoTauNu = cms.EDAnalyzer("DQMSimpleFileSaver",
                                                    outputFileName = cms.string('fitBgEstTemplateWtoTauNu_results.root'),
                                                    outputCommands = cms.vstring(
#            'drop /harvested/*',
#                    'keep /template/*',
#                    'keep /analysis/*/zMuTauAnalyzerOS/afterEvtSelDiTauCandidateForMuTauNonZeroCharge/*',
#                    'keep /fitResults/*'
                )
                                                )

process.p = cms.Path(
    process.loadAnalysisResultsWtoTauNu
    +process.subHist
    +process.normHist
    +process.fitWtoTauNu
    +process.saveFitResultsWtoTauNu
)
