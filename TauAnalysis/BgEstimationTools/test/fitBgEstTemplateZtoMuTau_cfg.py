import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Apply "template" method for data-driven background estimation
# to Z --> mu + tau-jet channel
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

process = cms.Process('fitBgEstTemplateZtoMuTau')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

dqmDirectoriesProcess = {
    'Ztautau' : 'ZtautauSum',
    'Zmumu' : 'Zmumu',
    'WplusJets' : 'WplusJets',
    'QCD' : 'qcdSum',
    'TTplusJets' : 'TTplusJets',
    'smSum' : 'smSum',
    'Data' : 'data'
}

dqmDirectoryAnalysis = 'zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVetoByMass/'

dqmDirectoriesBgEnrichedSelections = {
    'ZmumuJetMisIdEnriched' : 'BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched/afterDiMuonPairInvMassBgEstZmumuJetMisIdEnriched/',
    'ZmumuMuonMisIdEnriched' : 'BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched/afterDiMuonPairBgEstZmumuMuonMisIdEnriched/',
    'WplusJetsEnriched' : 'BgEstTemplateAnalyzer_WplusJetsEnriched/afterDiMuonVetoBgEstWplusJetsEnriched/',
    ##'TTplusJetsEnriched' : 'BgEstTemplateAnalyzer_TTplusJetsEnriched/afterJetEt60BgEstTTplusJetsEnriched/',
    'QCDenriched' : 'BgEstTemplateAnalyzer_QCDenriched/afterDiMuonVetoBgEstQCDenriched/'
}

pureProcessBgEnrichedSelections = {
    'ZmumuJetMisIdEnriched' : 'Zmumu',
    'ZmumuMuonMisIdEnriched' : 'Zmumu',
    'WplusJetsEnriched' : 'WplusJets',
    ##'TTplusJetsEnriched' : 'TTplusJets',
    'QCDenriched' : 'QCD',
}

meName_visMass = "DiTauCandidateQuantities/VisMass"
meName_visMass_norm = "DiTauCandidateQuantities/VisMassShape"
meName_SVfitMass = "DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass"
meName_SVfitMass_norm = "DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/MassShape"

#--------------------------------------------------------------------------------
# load template histogram for Z --> tau+ tau- signal and different types of background events,
# and distributions observed in (pseudo)data in events passing final analysis selection criteria
#--------------------------------------------------------------------------------

process.loadAnalysisResultsZtoMuTau = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = cms.PSet(
        inputFileNames = cms.vstring(
            getHarvestingFilePath('ZtoMuTau_bgEstTemplate') + '/' + 'bgEstTemplateHistZtoMuTau_skimmed.root'
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('/')
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

jobsSysBiasHist = []

for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
    dqmDirectoryData = dqmDirectoriesProcess['Data']
    dqmDirectoryAnalysisMC_pure = dqmDirectoriesProcess[pureProcessBgEnrichedSelections[bgEnrichedSelectionName]]

    jobsSysBiasHist.append(cms.PSet(
        meName_minuend = cms.string(
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure + '/' + dqmDirectoryAnalysis + meName_visMass_norm
        ),
        meName_subtrahend = cms.string(
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
        ),
        meName_difference = cms.string(
            'systematics/bias/' + dqmDirectoryAnalysisMC_pure \
           + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
        )
    ))
    jobsSysBiasHist.append(cms.PSet(
        meName_minuend = cms.string(
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure + '/' + dqmDirectoryAnalysis + meName_SVfitMass_norm
        ),
        meName_subtrahend = cms.string(
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
        ),
        meName_difference = cms.string(
            'systematics/bias/' + dqmDirectoryAnalysisMC_pure \
           + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
        )
    ))

process.prodSysBiasHistZtoMuTau = cms.EDAnalyzer("DQMHistSubtractor",
    config = cms.VPSet(jobsSysBiasHist)
)

#--------------------------------------------------------------------------------
# produce auxiliary histograms representing uncertainties on shape of template histograms
# caused by imprecise knowledge of template purity/
# contributions from Ztautau signal and other background processes to background enriched sample
#--------------------------------------------------------------------------------

jobsSysBgEnrichedSamplePurityHist = []

for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
    dqmDirectoryBgEstMC_pure = dqmDirectoriesProcess[pureProcessBgEnrichedSelections[bgEnrichedSelectionName]]
    dqmDirectoryBgEstMC_smSum = dqmDirectoriesProcess['smSum']

    jobsSysBgEnrichedSamplePurityHist.append(cms.PSet(
        meName_minuend = cms.string(
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
        ),
        meName_subtrahend = cms.string(
            '/template/harvested/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
        ),
        meName_difference = cms.string(
            'systematics/bgEnrichedSamplePurity/' + dqmDirectoryBgEstMC_pure \
           + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
        )
    ))
    jobsSysBgEnrichedSamplePurityHist.append(cms.PSet(                                             
        meName_minuend = cms.string(
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
        ),
        meName_subtrahend = cms.string(
            '/template/harvested/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
        ),
        meName_difference = cms.string(
            'systematics/bgEnrichedSamplePurity/' + dqmDirectoryBgEstMC_pure \
           + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
        )
    ))

process.prodSysBgEnrichedSamplePurityHistZtoMuTau = cms.EDAnalyzer("DQMHistSubtractor",
    config = cms.VPSet(jobsSysBgEnrichedSamplePurityHist)
)

#--------------------------------------------------------------------------------
# scale histograms representing systematic uncertainties to normalization of shape templates
# (normalization is needed in order to combine systematic + statistical uncertainties
#  to total uncertainty on shape templates)
#--------------------------------------------------------------------------------

jobsScaleSysHist = []

for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
    jobsScaleSysHist.append(cms.PSet(
        meName_input = cms.string(
            'systematics/bias/' + dqmDirectoryBgEstMC_pure \
           + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
        ),
        meName_output = cms.string(
            'systematics/bias/' + dqmDirectoryBgEstMC_pure \
           + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass
        ),
        meName_scaleFactor = cms.string(
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass
        ),
        meType = cms.string("hist")
    ))
    jobsScaleSysHist.append(cms.PSet(
        meName_input = cms.string(
            'systematics/bias/' + dqmDirectoryBgEstMC_pure \
           + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
        ),
        meName_output = cms.string(
            'systematics/bias/' + dqmDirectoryBgEstMC_pure \
           + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass
        ),
        meName_scaleFactor = cms.string(
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass
        ),
        meType = cms.string("hist")
    ))

    jobsScaleSysHist.append(cms.PSet(
        meName_input = cms.string(
            'systematics/bgEnrichedSamplePurity/' + dqmDirectoryBgEstMC_pure \
           + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
        ),
        meName_output = cms.string(
            'systematics/bgEnrichedSamplePurity/' + dqmDirectoryBgEstMC_pure \
           + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass
        ),
        meName_scaleFactor = cms.string(
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass
        ),
        meType = cms.string("hist")
    ))
    jobsScaleSysHist.append(cms.PSet(
        meName_input = cms.string(
            'systematics/bgEnrichedSamplePurity/' + dqmDirectoryBgEstMC_pure \
           + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
        ),
        meName_output = cms.string(
            'systematics/bgEnrichedSamplePurity/' + dqmDirectoryBgEstMC_pure \
           + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass
        ),
        meName_scaleFactor = cms.string(
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass
        ),
        meType = cms.string("hist")
    ))

process.scaleSysHistZtoMuTau = cms.EDAnalyzer("DQMHistScaler",
    config = cms.VPSet(jobsScaleSysHist)
)

#--------------------------------------------------------------------------------
# print-out bin contents of histograms
#--------------------------------------------------------------------------------

dumpSysHistMonitorElements = []

for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
    dqmDirectoryData = dqmDirectoriesProcess['Data']
    dqmDirectoryBgEstMC_pure = dqmDirectoriesProcess[pureProcessBgEnrichedSelections[bgEnrichedSelectionName]]
    dqmDirectoryAnalysisMC_pure = dqmDirectoriesProcess[pureProcessBgEnrichedSelections[bgEnrichedSelectionName]]
    
    dumpSysHistMonitorElements.append(
        '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure + '/' + dqmDirectoryAnalysis + meName_visMass_norm
    )
    dumpSysHistMonitorElements.append(
        '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
    )
    dumpSysHistMonitorElements.append(
        'systematics/bias/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
    )
    dumpSysHistMonitorElements.append(
        'systematics/bias/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass
    )
    dumpSysHistMonitorElements.append(
        'systematics/bgEnrichedSamplePurity/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
    )
    dumpSysHistMonitorElements.append(
        'systematics/bgEnrichedSamplePurity/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass
    )
    dumpSysHistMonitorElements.append(
        '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure + '/' + dqmDirectoryAnalysis + meName_SVfitMass_norm
    )
    dumpSysHistMonitorElements.append(
        '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
    )
    dumpSysHistMonitorElements.append(
        'systematics/bias/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
    )
    dumpSysHistMonitorElements.append(
        'systematics/bias/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass
    )
    dumpSysHistMonitorElements.append(
        'systematics/bgEnrichedSamplePurity/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
    )
    dumpSysHistMonitorElements.append(
        'systematics/bgEnrichedSamplePurity/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass
    )

process.dumpSysHistZtoMuTau = cms.EDAnalyzer("DQMDumpHistogram",
    meNames = cms.vstring(dumpSysHistMonitorElements)
)                                             

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

process.fitZtoMuTau = cms.EDAnalyzer("TemplateHistFitter",                                          
    processes = cms.PSet(
        Ztautau = cms.PSet(
            templates = cms.PSet(
                visMass = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/' + dqmDirectoriesProcess['Ztautau'] + '/' + dqmDirectoryAnalysis + meName_visMass_norm
                    ),
                    fitSimultaneously = cms.bool(False),                                 
                    smoothing = cms.PSet(
                        pluginName = cms.string("visMassSmoothingZtautau"),
                        pluginType = cms.string("SmoothGenericPdfWrapper"),
                        # fit Z --> tau+ tau- peak with sum of log-normal and skewed Gaussian distribution
                        formula = cms.string(
                            "par1*TMath::LogNormal(@x, par4, par2, par3)" \
                           + " + (1 - par1)*TMath::Gaus(@x, par5, par6)*(1 + TMath::Erf(par7*@x))"
                        ),
                        xMin = cms.double(20.),
                        xMax = cms.double(120.),
                        parameter = cms.PSet(
                            par1 = cms.PSet(
                                initial = cms.double(0.75),
                                min = cms.double(0.),
                                max = cms.double(1.)
                            ),
                            par2 = cms.PSet(
                                initial = cms.double(0.5),
                                min = cms.double(0.),
                                max = cms.double(10.)
                            ),
                            par3 = cms.PSet(
                                initial = cms.double(40.),
                                min = cms.double(0.),
                                max = cms.double(100.)
                            ),
                            par4 = cms.PSet(
                                initial = cms.double(10.),
                                min = cms.double(0.),
                                max = cms.double(100.)
                            ),
                            par5 = cms.PSet(
                                initial = cms.double(55.),
                                min = cms.double(0.),
                                max = cms.double(100.)
                            ),
                            par6 = cms.PSet(
                                initial = cms.double(10.),
                                min = cms.double(0.),
                                max = cms.double(100.)
                            ),
                            par7 = cms.PSet(
                                initial = cms.double(0.0001),
                                min = cms.double(0.),
                                max = cms.double(1.)
                            )
                        )
                    )
                ),
                SVfitMass = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/' + dqmDirectoriesProcess['Ztautau'] + '/' + dqmDirectoryAnalysis + meName_SVfitMass_norm
                    )
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(225.)
            ),
            drawOptions = copy.deepcopy(drawOption_Ztautau_separate)
        ),
        ZmumuJetMisId = cms.PSet(
            templates = cms.PSet(
                visMass = cms.PSet(
                    meName = cms.string(
                        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
                       + dqmDirectoriesBgEnrichedSelections['ZmumuJetMisIdEnriched'] + meName_visMass_norm
                    ),
                    fitSimultaneously = cms.bool(False),                                    
                    smoothing = visMass_smoothing.clone(
                        pluginName = cms.string("visMassSmoothingZmumuJetMisId"),
                        ##excludeBinsX = cms.vdouble(82.5, 87.5, 92.5, 97.5), # bins to exclude in case no rebinning is used
                        excludeBinsX = cms.vdouble(85., 95.),                 # bins to exclude in case two bins are combined into one
                        parameter = visMass_smoothing.parameter.clone(
                            width = visMass_smoothing.parameter.width.clone(
                                initial = cms.double(20.)
                            ),
                            mp = visMass_smoothing.parameter.mp.clone(
                                initial = cms.double(65.)
                            ),
                            gsigma = visMass_smoothing.parameter.gsigma.clone(
                                initial = cms.double(5.)
                            )
                        )
                    )
                ),
                SVfitMass = cms.PSet(
                    meName = cms.string(
                        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
                       + dqmDirectoriesBgEnrichedSelections['ZmumuJetMisIdEnriched'] + meName_visMass_norm
                    )
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(5.)
            ),
            drawOptions = copy.deepcopy(drawOption_Zmumu_separate)
        ),
        ZmumuMuonMisId = cms.PSet(
            templates = cms.PSet(
                visMass = cms.PSet(
                    meName = cms.string(
                        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
                       + dqmDirectoriesBgEnrichedSelections['ZmumuMuonMisIdEnriched'] + meName_visMass_norm
                    ),
                    fitSimultaneously = cms.bool(False),                                   
                    smoothing = cms.PSet(
                        pluginName = cms.string("visMassSmoothingZmumuMuonMisId"),
                        pluginType = cms.string("SmoothGenericPdfWrapper"),
                        # fit Z --> mu+ mu- peak with Voigt function,
                        # the convolution of a Breit-Wigner profile with a Gaussian (smearing)
                        formula = cms.string("TMath::Voigt(@x - x0, gsigma, gamma)"),
                        xMin = cms.double(80.),
                        xMax = cms.double(100.),                        
                        parameter = cms.PSet(
                            x0 = cms.PSet(
                                initial = cms.double(90.),
                                min = cms.double(80.),
                                max = cms.double(100.)
                            ),
                            gsigma = cms.PSet(
                                initial = cms.double(2.5),
                                min = cms.double(0.),
                                max = cms.double(10.)
                            ),
                            gamma = cms.PSet(
                                initial = cms.double(2.5),
                                min = cms.double(0.),
                                max = cms.double(10.)
                            )
                        )
                    )
                ),
                SVfitMass = cms.PSet(
                    meName = cms.string(
                        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
                       + dqmDirectoriesBgEnrichedSelections['ZmumuMuonMisIdEnriched'] + meName_visMass_norm
                    )
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(5.)
            ),
            drawOptions = copy.deepcopy(drawOption_Zmumu_separate)
        ),
        WplusJets = cms.PSet(
            templates = cms.PSet(
                visMass = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/' + dqmDirectoriesProcess['WplusJets'] + '/' + dqmDirectoryAnalysis + meName_visMass_norm
                    ),
                    fitSimultaneously = cms.bool(False),                                   
                    smoothing = visMass_smoothing.clone(
                        pluginName = cms.string("visMassSmoothingWplusJets")
                    )
                ),
                SVfitMass = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/' + dqmDirectoriesProcess['WplusJets'] + '/' + dqmDirectoryAnalysis + meName_SVfitMass_norm
                    )
                )    
            ),    
            norm = cms.PSet(
                initial = cms.double(25.)
            ),
            drawOptions = drawOption_WplusJets_separate 
        ),
        TTplusJets = cms.PSet(
            templates = cms.PSet(
                visMass = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/' + dqmDirectoriesProcess['TTplusJets'] + '/' + dqmDirectoryAnalysis + meName_visMass_norm
                    ),
                    fitSimultaneously = cms.bool(False),                                   
                    smoothing = visMass_smoothing.clone(
                        pluginName = cms.string("visMassSmoothingTTplusJets")
                    )
                ),
                SVfitMass = cms.PSet(
                    meName = cms.string(
                        '/analysis/harvested/' + dqmDirectoriesProcess['TTplusJets'] + '/' + dqmDirectoryAnalysis + meName_SVfitMass_norm
                    )
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(5.)
            ),
            drawOptions = copy.deepcopy(drawOption_TTplusJets_separate)
        ),
        QCD = cms.PSet(
            templates = cms.PSet(
                visMass = cms.PSet(
                    meName = cms.string(
                        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
                       + dqmDirectoriesBgEnrichedSelections['QCDenriched'] + meName_visMass_norm
                    ),
                    fitSimultaneously = cms.bool(False),                                    
                    smoothing = visMass_smoothing.clone(
                        pluginName = cms.string("visMassSmoothingQCD"),
                        xMax = cms.double(150.)
                    )
                ),
                SVfitMass = cms.PSet(
                    meName = cms.string(
                        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
                       + dqmDirectoriesBgEnrichedSelections['QCDenriched'] + meName_SVfitMass_norm
                    )
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(75.)
            ),
            drawOptions = copy.deepcopy(drawOption_QCD_separate)
        )
    ),

    # use "pseudo" data-samples consisting of all Monte Carlo processes for testing                      
    data = cms.PSet(
        distributions = cms.PSet(
            visMass = cms.PSet(
                meName = cms.string(
                    '/analysis/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoryAnalysis + meName_visMass
                )
            ),
            SVfitMass = cms.PSet(
                meName = cms.string(
                    '/analysis/harvested/' + dqmDirectoriesProcess['Data'] + '/' + dqmDirectoryAnalysis + meName_SVfitMass
                )
            )
        )
    ),

    fit = cms.PSet(
        algorithm = cms.PSet(
            pluginName = cms.string("fitBgEstTemplateZtoMuTauAlgorithm"),
            #pluginType = cms.string("TemplateFitAdapter_TFractionFitter")
            pluginType = cms.string("TemplateFitAdapter_RooFit")
        ),
        variables = cms.PSet(
            visMass = cms.PSet(
                name = cms.string("visMass"),
                title = cms.string("M_{vis}(Muon + Tau)"),
                min = cms.double(20.), # default:  20.
                max = cms.double(200.) # default: 200.
            )
            ##SVfitMass = cms.PSet(
            ##    name = cms.string("SVfitMass"),
            ##    title = cms.string("M(Muon + Tau), SVfit method"),
            ##    min = cms.double(40.), # default:  40.
            ##    max = cms.double(200.) # default: 200.
            ##)
        ),
        # constrain normalization of W + jets, ttbar + jets and QCD backgrounds
        # to Monte Carlo expectation multiplied by "k-factors" determined
        # in background enriched samples
        constraints = cms.PSet(
            ##ZmumuJetMisId = cms.PSet(
            ##    norm = cms.PSet(
            ##        value = cms.double(1.*1.),
            ##        uncertainty = cms.double(5.)
            ##    )
            ##),
            ##ZmumuMuonMisId = cms.PSet(
            ##    norm = cms.PSet(
            ##        value = cms.double(1.*2.5),
            ##        uncertainty = cms.double(5.)
            ##    )
            ##),
            ##WplusJets = cms.PSet(
            ##    norm = cms.PSet(
            ##        value = cms.double(1.*25.),
            ##        uncertainty = cms.double(25.)
            ##    )
            ##),
            ##TTplusJets = cms.PSet(
            ##    norm = cms.PSet(
            ##        value = cms.double(1.*2.5),
            ##        uncertainty = cms.double(5.)
            ##    )
            ##),
            ##QCD = cms.PSet(
            ##    norm = cms.PSet(
            ##        value = cms.double(1.*75.),
            ##        uncertainty = cms.double(37.5)
            ##    )
            ##)
        ),
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

    estSysUncertainties = cms.PSet(
        fluctuations = cms.PSet(
            ##bias = cms.PSet(
            ##    meNames = cms.PSet(
            ##        Ztautau = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        ZmumuJetMisId = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        ZmumuMuonMisId = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        WplusJets = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        TTplusJets = cms.PSet(
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
            ##        ZmumuJetMisId = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        ZmumuMuonMisId = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        WplusJets = cms.PSet(
            ##            visMass = cms.string(),
            ##            SVfitMass = cms.string(),
            ##        ),
            ##        TTplusJets = cms.PSet(
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
        ),       
        numSamplings = cms.int32(0),
        chi2redMax = cms.double(10.),
        verbosity = cms.PSet(
            printLevel = cms.int32(-1),
            printWarnings = cms.bool(False)
        )
    ),                                     

    output = cms.PSet(
        controlPlots = cms.PSet(
            fileName = cms.string("./plots/fitBgEstTemplateZtoMuTau_#PLOT#.png")
        ),
        fitResults = cms.PSet(
            dqmDirectory = cms.string('/fitResults')
        )
    )                                      
)                          

##process.fitZtoMuTauSysTauEnUp = copy.deepcopy(process.fitZtoMuTau)
##process.fitZtoMuTauSysTauEnUp.processes.Ztautau.templates.visMass.meName = \
##  cms.string(dqmDirectory_Ztautau_templateSysTauEnUp + '/' + meName_visMass)
##process.fitZtoMuTauSysTauEnUp.output.controlPlots.fileName = \
##  cms.string("./plots/fitBgEstTemplateZtoMuTau_#PLOT#_sysTauEnUp.png")

##process.fitZtoMuTauSysTauEnDown = copy.deepcopy(process.fitZtoMuTau)
##process.fitZtoMuTauSysTauEnDown.processes.Ztautau.templates.visMass.meName = \
##  cms.string(dqmDirectory_Ztautau_templateSysTauEnDown + '/' + meName_visMass)
##process.fitZtoMuTauSysTauEnDown.output.controlPlots.fileName = \
##  cms.string("./plots/fitBgEstTemplateZtoMuTau_#PLOT#_sysTauEnDown.png")

process.saveFitResultsZtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('fitBgEstTemplateZtoMuTau_results.root'),
    outputCommands = cms.vstring(
        'drop /harvested/*',
        'keep /template/*',
        'keep /analysis/*/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVetoByMass/*',
        'keep /fitResults/*'
    )
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.p = cms.Path(
    process.loadAnalysisResultsZtoMuTau
   ##+ process.dumpDQMStore 
   #+ process.prodSysBiasHistZtoMuTau
   #+ process.prodSysBgEnrichedSamplePurityHistZtoMuTau
   #+ process.scaleSysHistZtoMuTau
   #+ process.dumpSysHistZtoMuTau
   + process.fitZtoMuTau
   #+ process.fitZtoMuTauSysTauEnUp + process.fitZtoMuTauSysTauEnDown 
   + process.saveFitResultsZtoMuTau
)

# print-out all python configuration parameter information
print process.dumpPython()


  
