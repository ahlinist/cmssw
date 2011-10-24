import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Apply "template" method for data-driven background estimation
# to Z --> elec + tau-jet channel
#
# Authors: Christian Veelken, UC Davis
#          Jeff Kolb, Notre Dame
#
#--------------------------------------------------------------------------------

from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *
from TauAnalysis.BgEstimationTools.templateHistDefinitions_cfi import \
  drawJobTemplateHist, drawJobAnalysisHistBias, drawJobAnalysisHistPurity, \
  plotBgEstData, plotBgEstMC_pure, plotBgEstMC_smSum, plotAnalysisMC_pure
from TauAnalysis.BgEstimationTools.tools.drawTemplateHistConfigurator import drawTemplateHistConfigurator
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, getJobId

process = cms.Process('fitBgEstTemplateZtoElecTau')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

dqmDirectoriesProcess = {
    'Ztautau'        : 'ZtautauSum',
    'Zee'            : 'ZeeSum',
    'WplusJets'      : 'WplusJets_madgraph_pat',
    'QCD'            : 'qcdSum',
    'TTplusJets'     : 'TTplusJets_madgraph_pat',
##     'PhotonPlusJets' : 'photonPlusJetsSum',
##     'smSum'          : 'smSum',
    'Data'           : 'data'
}

dqmDirectoryAnalysis = 'BgEstTemplateAnalyzer_ZtautauEnriched/afterDiTauCandidateZeeHypothesisVetoBgEstZtautauEnriched/'

dqmDirectoriesBgEnrichedSelections = {
    'ZeeElectronMisIdEnriched' : 'BgEstTemplateAnalyzer_ZeeElectronMisIdEnriched/afterDiTauCandidateZeeHypothesisVetoBgEstZeeElectronMisIdEnriched/',
    'WplusJetsEnriched'        : 'BgEstTemplateAnalyzer_WplusJetsEnriched/afterDiTauCandidateZeeHypothesisVetoBgEstWplusJetsEnriched/',
#    'PhotonPlusJetsEnriched'   : 'BgEstTemplateAnalyzer_PhotonPlusJetsEnriched/afterDiTauCandidateZeeHypothesisVetoBgEstPhotonPlusJetsEnriched/',
    'QCDenriched'              : 'BgEstTemplateAnalyzer_QCDenriched/afterDiTauCandidateZeeHypothesisVetoBgEstQCDenriched/'
}

## pureProcessBgEnrichedSelections = {
##     'ZeeElectronMisIdEnriched' : 'Zee',
##     'WplusJetsEnriched'        : 'WplusJets',
##     'PhotonPlusJetsEnriched'   : 'PhotonPlusJets',
##     'QCDenriched'              : 'QCD'
## }

meNames = {
    'visMass'   : 'DiTauCandidateQuantities/VisMass',
    'SVfitMassInt' : 'DiTauCandidateNSVfitQuantities/psKine_MEt_logM_int/Mass',
    'SVfitMassFit' : 'DiTauCandidateNSVfitQuantities/psKine_MEt_logM_fit/Mass',    
}

#--------------------------------------------------------------------------------
# load template histogram for Z --> tau+ tau- signal and different types of background events,
# and distributions observed in (pseudo)data in events passing final analysis selection criteria
#--------------------------------------------------------------------------------

process.loadAnalysisResultsZtoElecTau = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = cms.PSet(
        inputFileNames = cms.vstring(
           'file:/afs/crc.nd.edu/user/l/lantonel/CMSSW_4_2_8/src/TauAnalysis/BgEstimationTools/test/Run01/bgEstTemplateHistZtoElecTau_VisMass.root'
 
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('/')
#        dqmDirectory_store = cms.string('/harvested/ZtautauSum' + '/' + dqmDirectories_control['ZtautauEnriched'])
    )
)

#--------------------------------------------------------------------------------
# produce auxiliary histograms representing bias of visible muon + tau-jet mass distribution
# introduced by differences in event selection between background enriched samples
# and final Z --> muon + tau-jet analysis
#
# NOTE:
#  minuend    = contribution of (pure) signal/background process expected in final analysis
#               (estimated by Monte Carlo)
#  subtrahend = template histogram taken from background enriched sample,
#               including contributions from other signal/background processes
#               (determined by (pseudo)data)
#  difference = minuend - subtrahend
#
# --> in order to account for bias between distribution observed in final analysis
#     and the shapes of signal/background templates fitted to that distribution
#     one needs an **upward** fluctuation of the histogram representing the difference,
#     using a Gaussian of mean 0. and variance 1.
#
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# fit template histograms to distribution of visible muon + tau-jet mass in (pseudo)Data,
# in order to determine normalization factors of individual background processes
#--------------------------------------------------------------------------------

# fit W + jets, ttbar + jets and QCD background templates
# with Landau distribution convoluted with Gaussian
visMass_smoothing = cms.PSet(
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

#--------------------------------------------------------------------------------
# WARNING: must **always** specify initial, min and max parameters for every smoothing/fit parameter !!
#         (otherwise RooFit will not fit the parameter properly)
#--------------------------------------------------------------------------------

process.fitZtoElecTau = cms.EDAnalyzer("TemplateHistFitter",                                          
    processes = cms.PSet(
        Ztautau = cms.PSet(
            templates = cms.PSet(
                visMass = cms.PSet(
                    meName = cms.string(
                        '/harvested/' + dqmDirectoriesProcess['Ztautau'] + '/' + dqmDirectoryAnalysis \
						+ meNames['visMass'] + 'Shape' + '_' + 'ZtautauEnriched'
##                         '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoryAnalysis \
## 						+ meNames['visMass'] + 'Shape' + '_' + 'ZtautauEnriched'

                    ),
                    fitSimultaneously = cms.bool(False),                                 
                    ## smoothing = cms.PSet(
##                         pluginName = cms.string("visMassSmoothingZtautau"),
##                         pluginType = cms.string("SmoothGenericPdfWrapper"),
##                         # fit Z --> tau+ tau- peak with sum of log-normal and skewed Gaussian distribution
##                         formula = cms.string(
##                             "par1*TMath::LogNormal(@x, par4, par2, par3)" \
##                            + " + (1 - par1)*TMath::Gaus(@x, par5, par6)*(1 + TMath::Erf(par7*@x))"
##                         ),
##                         xMin = cms.double(20.),
##                         xMax = cms.double(120.),
##                         parameter = cms.PSet(
##                             par1 = cms.PSet(
##                                 initial = cms.double(0.75),
##                                 min = cms.double(0.),
##                                 max = cms.double(1.)
##                             ),
##                             par2 = cms.PSet(
##                                 initial = cms.double(0.5),
##                                 min = cms.double(0.),
##                                 max = cms.double(10.)
##                             ),
##                             par3 = cms.PSet(
##                                 initial = cms.double(40.),
##                                 min = cms.double(0.),
##                                 max = cms.double(100.)
##                             ),
##                             par4 = cms.PSet(
##                                 initial = cms.double(10.),
##                                 min = cms.double(0.),
##                                 max = cms.double(100.)
##                             ),
##                             par5 = cms.PSet(
##                                 initial = cms.double(55.),
##                                 min = cms.double(0.),
##                                 max = cms.double(100.)
##                             ),
##                             par6 = cms.PSet(
##                                 initial = cms.double(10.),
##                                 min = cms.double(0.),
##                                 max = cms.double(100.)
##                             ),
##                             par7 = cms.PSet(
##                                 initial = cms.double(0.0001),
##                                 min = cms.double(0.),
##                                 max = cms.double(1.)
##                             )
##                         )
##                    )
                ),
                SVfitMassInt = cms.PSet(
                    meName = cms.string(
                        '/harvested/' + dqmDirectoriesProcess['Ztautau'] + '/' + dqmDirectoryAnalysis \
                       + meNames['SVfitMassInt'] 
                    )
                ),
                SVfitMassFit = cms.PSet(
                    meName = cms.string(
                        '/harvested/' + dqmDirectoriesProcess['Ztautau'] + '/' + dqmDirectoryAnalysis \
                       + meNames['SVfitMassFit'] 
                    )
                ),                
            ),    
            norm = cms.PSet(
                initial = cms.double(4177)
            ),
            drawOptions = copy.deepcopy(drawOption_Ztautau_separate)
        ),
        ZeeElectronMisId = cms.PSet(
            templates = cms.PSet(
                visMass = cms.PSet(
                    meName = cms.string(
                        '/harvested/' + dqmDirectoriesProcess['Zee'] + '/' + dqmDirectoryAnalysis \
						+ meNames['visMass'] + 'Shape' + '_' + 'ZtautauEnriched'
    
##                         '/harvested/' + dqmDirectoriesProcess['Zee'] + '/' + dqmDirectoriesBgEnrichedSelections['ZeeElectronMisIdEnriched'] \
##                        + meNames['visMass'] + 'Shape' + '_' + 'ZeeElectronMisIdEnriched'
                        
##                         '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoriesBgEnrichedSelections['ZeeElectronMisIdEnriched'] \
##                        + meNames['visMass'] + 'Shape' + '_' + 'ZeeElectronMisIdEnriched'
##                         '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoriesBgEnrichedSelections['ZeeElectronMisIdEnriched'] \
##                        + meNames['visMass'] + 'CorrectedShape'
                    ),
                    fitSimultaneously = cms.bool(False),
					## smoothing = cms.PSet(
## 					    pluginName = cms.string("visMassSmoothingZeeElectronMisId"),
## 					    pluginType = cms.string("SmoothGenericPdfWrapper"),
## 					    # fit Z --> e+ e- peak with Voigt function,
## 					    # the convolution of a Breit-Wigner profile with a Gaussian (smearing)
## 					    formula = cms.string("TMath::Voigt(@x - x0, gsigma, gamma)"),
## 					    xMin = cms.double(70.),
## 					    xMax = cms.double(110.),                        
## 					    parameter = cms.PSet(
## 					        x0 = cms.PSet(
## 					            initial = cms.double(90.),
## 					            min = cms.double(80.),
## 					            max = cms.double(100.)
## 					        ),
## 					        gsigma = cms.PSet(
## 					            initial = cms.double(0.1),
## 					            min = cms.double(0.),
## 					            max = cms.double(5.)
## 					        ),
## 					        gamma = cms.PSet(
## 					            initial = cms.double(2.5),
## 					            min = cms.double(0.),
## 					            max = cms.double(10.)
## 					        )
## 					    )
## 					)
                ),
                SVfitMassInt = cms.PSet(
                    meName = cms.string(
##                         '/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
##                         + dqmDirectoriesBgEnrichedSelections['ZeeElectronMisIdEnriched'] \
##                         + meNames['SVfitMassInt'] + 'Shape' + '_' + 'ZeeElectronMisIdEnriched'
    
##                         '/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
##                         + dqmDirectoriesBgEnrichedSelections['ZeeElectronMisIdEnriched'] \
##                         + meNames['SVfitMassInt'] + 'CorrectedShape'

                        '/harvested/' + dqmDirectoriesProcess['Zee'] + '/' \
                        + dqmDirectoryAnalysis \
                        + meNames['SVfitMassInt'] + 'Shape' + '_' + 'ZtautauEnriched'
                        
                    )
                ),
                SVfitMassFit = cms.PSet(
                    meName = cms.string(
##                         '/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
##                         + dqmDirectoriesBgEnrichedSelections['ZeeElectronMisIdEnriched'] \
##                         + meNames['SVfitMassFit'] + 'Shape' + '_' + 'ZeeElectronMisIdEnriched'
    
##                         '/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
##                         + dqmDirectoriesBgEnrichedSelections['ZeeElectronMisIdEnriched'] \
##                         + meNames['SVfitMassFit'] + 'CorrectedShape'
    
                        '/harvested/' + dqmDirectoriesProcess['Zee'] + '/' \
                        + dqmDirectoryAnalysis \
                        + meNames['SVfitMassFit'] + 'Shape' + '_' + 'ZtautauEnriched'

                        
                    )
                ),                
            ),    
            norm = cms.PSet(
                initial = cms.double(2250)
            ),
            drawOptions = copy.deepcopy(drawOption_Zee_separate)
        ),
        WplusJets = cms.PSet(
            templates = cms.PSet(
                visMass = cms.PSet(
                    meName = cms.string(
## 						 '/harvested/' + dqmDirectoriesProcess['WplusJets'] + '/' + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] \
## 						+ meNames['visMass']  + 'Shape' + '_' + 'WplusJetsEnriched'
						 '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] \
						+ meNames['visMass']  + 'Shape' + '_' + 'WplusJetsEnriched'
## 						 '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] \
## 						+ meNames['visMass']  + 'CorrectedShape' 
                                                 

                    ),
                    fitSimultaneously = cms.bool(False),                                   
##                     smoothing = visMass_smoothing.clone(
##                         pluginName = cms.string("visMassSmoothingWplusJets")
##                     )
                ),
                SVfitMassInt = cms.PSet(
                    meName = cms.string(
                        '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] \
                        + meNames['SVfitMassInt']  + 'Shape' + '_' + 'WplusJetsEnriched'
##                         '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] \
##                         + meNames['SVfitMassInt']  + 'CorrectedShape'

                    )
                ),
                SVfitMassFit = cms.PSet(
                    meName = cms.string(
                        '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] \
                        + meNames['SVfitMassFit']  + 'Shape' + '_' + 'WplusJetsEnriched'
##                         '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] \
##                         + meNames['SVfitMassFit']  + 'CorrectedShape'
                    )
                ),                 
                
            ),    
            norm = cms.PSet(
                initial = cms.double(2369)
            ),
            drawOptions = drawOption_WplusJets_separate 
        ),
##         PhotonPlusJets = cms.PSet(
##             templates = cms.PSet(
##                 visMass = cms.PSet(
##                     meName = cms.string(
##                         '/harvested/' + dqmDirectoriesProcess['PhotonPlusJets'] + '/' + dqmDirectoriesBgEnrichedSelections['PhotonPlusJetsEnriched'] \
##                        + meNames['visMass'] + 'Shape' + '_' + 'PhotonPlusJetsEnriched'
## ##                         '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoriesBgEnrichedSelections['PhotonPlusJetsEnriched'] \
## ##                         + meNames['visMass'] + 'Shape' + '_' + 'PhotonPlusJetsEnriched'

##                     ),
##                     fitSimultaneously = cms.bool(False),                                   
##                     smoothing = visMass_smoothing.clone(
##                         pluginName = cms.string("visMassSmoothingPhotonPlusJets")
##                     )
##                 ),
## ##                 SVfitMass = cms.PSet(
## ##                     meName = cms.string(
## ##                         '/harvested/' + dqmDirectoriesProcess['PhotonPlusJets'] + '/' + dqmDirectoryAnalysis \
## ##                        + meNames['visMass'] 
## ##                     )
## ##                 )
##             ),    
##             norm = cms.PSet(
##                 initial = cms.double(206.7)
##             ),
##             drawOptions = copy.deepcopy(drawOption_gammaPlusJets_separate)
##         ),
        QCD = cms.PSet(
            templates = cms.PSet(
                visMass = cms.PSet(
                    meName = cms.string(
##                         '/harvested/' + dqmDirectoriesProcess['QCD'] + '/' + dqmDirectoriesBgEnrichedSelections['QCDenriched'] \
##                        + meNames['visMass'] + 'Shape' + '_' + 'QCDenriched'
                        '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoriesBgEnrichedSelections['QCDenriched'] \
                       + meNames['visMass'] + 'Shape' + '_' + 'QCDenriched'

                    ),
                    fitSimultaneously = cms.bool(False),                                    
##                     smoothing = visMass_smoothing.clone(
##                         pluginName = cms.string("visMassSmoothingQCD"),
##                         xMax = cms.double(150.)
##                     )
                ),
                SVfitMassInt = cms.PSet(
                    meName = cms.string(
                        '/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
                        + dqmDirectoriesBgEnrichedSelections['QCDenriched'] \
                        + meNames['SVfitMassInt'] + 'Shape' + '_' + 'QCDenriched'
                    )
                ),
                SVfitMassFit = cms.PSet(
                    meName = cms.string(
                        '/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
                        + dqmDirectoriesBgEnrichedSelections['QCDenriched'] \
                        + meNames['SVfitMassFit'] + 'Shape' + '_' + 'QCDenriched'
                    )
                ),                
            ),    
            norm = cms.PSet(
                initial = cms.double(5590)
            ),
            drawOptions = copy.deepcopy(drawOption_QCD_separate)
        ),
        TTplusJets = cms.PSet(
            templates = cms.PSet(
                visMass = cms.PSet(
                    meName = cms.string(
                       '/harvested/' + dqmDirectoriesProcess['TTplusJets'] + '/' + dqmDirectoryAnalysis \
                       + meNames['visMass'] + 'Shape' + '_' + 'ZtautauEnriched'
                    ),
                    fitSimultaneously = cms.bool(False),                                    
##                     smoothing = visMass_smoothing.clone(
##                         pluginName = cms.string("visMassSmoothingQCD"),
##                         xMax = cms.double(150.)
##                     )
                ),
                
                SVfitMassInt = cms.PSet(
                    meName = cms.string(
                       '/harvested/' + dqmDirectoriesProcess['TTplusJets'] + '/' + dqmDirectoryAnalysis \
                       + meNames['SVfitMassInt'] + 'Shape' + '_' + 'ZtautauEnriched'
                    )
                ),
                SVfitMassFit = cms.PSet(
                    meName = cms.string(
                       '/harvested/' + dqmDirectoriesProcess['TTplusJets'] + '/' + dqmDirectoryAnalysis \
                       + meNames['SVfitMassFit'] + 'Shape' + '_' + 'ZtautauEnriched'
                    )
                ),                
            ),    
            norm = cms.PSet(
                initial = cms.double(253)
            ),
            drawOptions = copy.deepcopy(drawOption_TTplusJets_separate)
        )  
        
    ),

    # use "pseudo" data-samples consisting of all Monte Carlo processes for testing                      
    data = cms.PSet(
        distributions = cms.PSet(
            visMass = cms.PSet(
                meName = cms.string(
                    '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoryAnalysis \
                   + meNames['visMass']
                )
            ),
            SVfitMassInt = cms.PSet(
                meName = cms.string(
                    '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoryAnalysis \
                   + meNames['SVfitMassInt']
                )
            ),
            SVfitMassFit = cms.PSet(
                meName = cms.string(
                    '/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoryAnalysis \
                   + meNames['SVfitMassFit']
                )
            ),            
        )
    ),

    fit = cms.PSet(
        algorithm = cms.PSet(
            pluginName = cms.string("fitBgEstTemplateZtoElecTauAlgorithm"),
            #pluginType = cms.string("TemplateFitAdapter_TFractionFitter")
            pluginType = cms.string("TemplateFitAdapter_RooFit")
        ),
        variables = cms.PSet(
			visMass = cms.PSet(
			   name = cms.string("visMass"),
			   title = cms.string("M_{vis}(Electron + Tau)"),
			   min = cms.double(20.), # default:  20.
			   max = cms.double(200.) # default: 200.
			),
## 			SVfitMassInt = cms.PSet(
## 			    name = cms.string("SVfitMassInt"),
## 			    title = cms.string("M(Electron + Tau), SVfit method"),
## 			    min = cms.double(40.), # default:  40.
## 			    max = cms.double(200.) # default: 200.
## 			),
## 			SVfitMassFit = cms.PSet(
## 			    name = cms.string("SVfitMassFit"),
## 			    title = cms.string("M(Electron + Tau), SVfit method"),
## 			    min = cms.double(40.), # default:  40.
## 			    max = cms.double(200.) # default: 200.
## 			), 
                        
        ),
        # constrain normalization of W + jets, ttbar + jets and QCD backgrounds
        # to Monte Carlo expectation multiplied by "k-factors" determined
        # in background enriched samples
##         constraints = cms.PSet(
##             Ztautau = cms.PSet(
##                 norm = cms.PSet(
##                     value = cms.double(4177),
##                     uncertainty = cms.double(4177)
##                 )
##             ),    
##             ZeeElectronMisId = cms.PSet(
##                 norm = cms.PSet(
##                     value = cms.double(2250),
##                     uncertainty = cms.double(2250)
##                 )
##             ),
##             WplusJets = cms.PSet(
##                 norm = cms.PSet(
##                     value = cms.double(2369),   
##                     uncertainty = cms.double(0.01*2369) 
##                 )
##             ),
##             QCD = cms.PSet(
##                 norm = cms.PSet(
##                     value = cms.double(5590), 
##                     uncertainty = cms.double(5590)
##                 )
##             ),
##             TTplusJets = cms.PSet(
##                 norm = cms.PSet(
##                     value = cms.double(253), 
##                     uncertainty = cms.double(10)
##                 )
##             )    
##         ),
        cutUnfittedRegion = cms.bool(False),
        #cutUnfittedRegion = cms.bool(True),
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

	#estSysUncertainties = cms.PSet(
	#    fluctuations = cms.PSet(
            ##bias = cms.PSet(
            ##    meNames = cms.PSet(
            ##        Ztautau = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        ZeeElectronMisId = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        WplusJets = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        PhotonPlusJets = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        QCD = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        )
            ##    ),
            ##    pullRMS = cms.double(1.),
            ##    pullMin = cms.double(0.),
            ##    pullMax = cms.double(1.),
            ##    mode = cms.string("coherent") # coherent/incoherent
            ##),
            ##bgSamplePurity = cms.PSet(
            ##    meNames = cms.PSet(
            ##        Ztautau = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        ZeeElectronMisId = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        WplusJets = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        PhotonPlusJets = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        QCD = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        )
            ##    ),             
            ##    pullRMS = cms.double(1.),
            ##    pullMin = cms.double(-1.),
            ##    pullMax = cms.double(+2.),
            ##    mode = cms.string("coherent")
            ##)
   #     ),       
   #     numSamplings = cms.int32(0),
   #     chi2redMax = cms.double(10.),
   #     verbosity = cms.PSet(
   #         printLevel = cms.int32(-1),
   #         printWarnings = cms.bool(False)
   #     )
   # ),                                     

    output = cms.PSet(
        controlPlots = cms.PSet(
            fileName = cms.string("./plots/fitBgEstTemplateZtoElecTau_#PLOT#.png")
        ),
        fitResults = cms.PSet(
            dqmDirectory = cms.string('/fitResults')
        )
    )                                      
)                          

##process.fitZtoElecTauSysTauEnUp = copy.deepcopy(process.fitZtoElecTau)
##process.fitZtoElecTauSysTauEnUp.processes.Ztautau.templates.visMass.meName = \
##  cms.string(dqmDirectory_Ztautau_templateSysTauEnUp + '/' + meName_visMass)
##process.fitZtoElecTauSysTauEnUp.output.controlPlots.fileName = \
##  cms.string("./plots/fitBgEstTemplateZtoElecTau_#PLOT#_sysTauEnUp.pdf")

##process.fitZtoElecTauSysTauEnDown = copy.deepcopy(process.fitZtoElecTau)
##process.fitZtoElecTauSysTauEnDown.processes.Ztautau.templates.visMass.meName = \
##  cms.string(dqmDirectory_Ztautau_templateSysTauEnDown + '/' + meName_visMass)
##process.fitZtoElecTauSysTauEnDown.output.controlPlots.fileName = \
##  cms.string("./plots/fitBgEstTemplateZtoElecTau_#PLOT#_sysTauEnDown.pdf")

process.saveFitResultsZtoElecTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string( getHarvestingFilePath('ZtoElecTau_bgEstTemplate') + '/' +'fitBgEstTemplateZtoElecTau_results.root'),
    outputCommands = cms.vstring(
           'keep *'
##         'drop /harvested/*',
##         'keep /template/*',
##         'keep /analysis/*/',
##         'keep /fitResults/*'
    )
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.p = cms.Path(
    process.loadAnalysisResultsZtoElecTau
   ##+ process.dumpDQMStore 
   #+ process.prodSysBiasHistZtoElecTau
   #+ process.prodSysBgEnrichedSamplePurityHistZtoElecTau
   #+ process.scaleSysHistZtoElecTau
   #+ process.dumpSysHistZtoElecTau
   + process.fitZtoElecTau
   #+ process.fitZtoElecTauSysTauEnUp + process.fitZtoElecTauSysTauEnDown 
   + process.saveFitResultsZtoElecTau
)

# print-out all python configuration parameter information
#print process.dumpPython()


  
