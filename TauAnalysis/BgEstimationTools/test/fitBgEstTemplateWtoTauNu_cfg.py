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
    'Data'       : 'data'
    }

dqmDirectoryAnalysis = 'wTauNuAnalyzer/afterEvtSelHtRatio_beforeEvtSelMetTopology/'
#dqmDirectoryAnalysis = 'wTauNuAnalyzer/afterEvtSelPFMetPt_beforeEvtSelHtRatio/'

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
        ewkSum = cms.PSet(
            templates = cms.PSet(
                Mt = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/'+ dqmDirectoriesProcess['ewkSum'] + '/' + dqmDirectoryAnalysis + meNames['Mt']
                        )
                    #fitSimultaneously = cms.bool(False),
                    #smoothing = transMass_smoothing.clone(
                    #    pluginName = cms.string("transMassSmoothingEwkSum"),
                    #    xMax = cms.double(150.)
                    #   )
                    ),
                MtSVfit = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/'+ dqmDirectoriesProcess['ewkSum'] + '/' + dqmDirectoryAnalysis + meNames['MtSVfit']
                        )
                    ),
                MEtTopology = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/' + dqmDirectoriesProcess['ewkSum'] + '/' + dqmDirectoryAnalysis + meNames['MEtTopology']
                        )
                    )
                ),
            norm = cms.PSet(
                initial = cms.double(5.)
                ),
            drawOptions = copy.deepcopy(drawOption_Wtaunu_separate)
            ),
        QCD = cms.PSet(
            templates = cms.PSet(
                Mt = cms.PSet( 
                    meName = cms.string(
                        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
                            + dqmDirectoriesBgEnrichedSelections['QCDEnriched'] \
                            + meNames['Mt'] + 'Shape' + '_' + 'QCDEnriched'
                        )
                    #fitSimultaneously = cms.bool(False),
                    #smoothing = transMass_smoothing.clone(
                    #    pluginName = cms.string("transMassSmoothingQCD"),
                    #    xMax = cms.double(150.)
                    #    )
                    ),
                MtSVfit = cms.PSet(
                    meName = cms.string(
                        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
                            + dqmDirectoriesBgEnrichedSelections['QCDEnriched'] \
                            + meNames['MtSVfit'] + 'Shape' + '_' + 'QCDEnriched'
                        )
                    ),
                MEtTopology = cms.PSet(
                    meName = cms.string(
                        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
                            +dqmDirectoriesBgEnrichedSelections['QCDEnriched'] \
                            +meNames['MEtTopology'] + 'Shape' + '_' + 'QCDEnriched'
                        )
                    )
                ),
            norm = cms.PSet(
                initial = cms.double(75.)
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
                        + meNames['Mt'] 
                    )
                ),
            MtSVfit = cms.PSet(
                meName = cms.string(
                    '/analysis/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoryAnalysis \
                        + meNames['MtSVfit'] 
                    )
                ),
            MEtTopology = cms.PSet(
                meName = cms.string(
                    '/analysis/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoryAnalysis \
                        + meNames['MEtTopology']
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
#                min = cms.double(20.),
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
        chi2redMax = cms.double(10),
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
    +process.fitWtoTauNu
    +process.saveFitResultsWtoTauNu
)
