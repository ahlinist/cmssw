import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *
from TauAnalysis.BgEstimationTools.templateHistDefinitions_cfi import \
  drawJobTemplateHist, drawJobTemplateHistIntegrated, drawJobAnalysisHistData, drawJobAnalysisHistMC, drawJobAnalysisHistZmumuEmbedding, \
  plotBgEstData, plotBgEstMC_pure, plotBgEstMC_smSum, plotAnalysisMC_pure, plotAnalysis_ZmumuEmbedding
from TauAnalysis.BgEstimationTools.tools.drawTemplateHistConfigurator import drawTemplateHistConfigurator
from TauAnalysis.Configuration.plotZtoMuTau_drawJobs_cfi import plots_ZtoMuTau
from TauAnalysis.DQMTools.tools.drawJobConfigurator import *
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles
from TauAnalysis.Configuration.makePlots2_grid import dqmHistPlotter_template
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, getJobId

process = cms.Process('makeBgEstTemplateZtoMuTauPlots')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

dqmDirectoriesProcess = {
    'Ztautau' : {
        'template' : 'ZtautauSum',
        'analysis' : 'ZtautauSum'
    },
    'Ztautau_from_ZmumuEmbedding' : {
        'template' : 'ZtoMuTau_from_ZmumuEmbedding',
        'analysis' : 'ZtoMuTau_from_ZmumuEmbedding'
    },
    'Zmumu' : {
        'template' : 'Zmumu',
        'analysis' : 'Zmumu'
    },
    'WplusJets' : {
        'template' : 'WplusJetsSum',
        'analysis' : 'WplusJetsSum'
    },
    'QCD' : {
        'template' : 'qcdSum',
        'analysis' : 'qcdSum'
    },
    'TTplusJets' : {
        'template' : 'TTplusJets',
        'analysis' : 'TTplusJets'
    },
    'smSum' : {
        'template' : 'smSum',
        'analysis' : 'smSum'
    },
    'Data' : {
        'template' : 'data',
        'analysis' : 'data'
    }
}

dqmDirectoryAnalysis = 'zMuTauAnalyzerOS/afterEvtSelDiTauCandidateForMuTauZeroCharge/'

rebinningAnalysis = {
    'visMass' : 2,
    'SVfitMass' : 2
}

dqmDirectoriesBgEnrichedSelections = {
    'ZmumuJetMisIdEnriched' : 'BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched/afterDiMuonPairInvMassBgEstZmumuJetMisIdEnriched/',
    'ZmumuMuonMisIdEnriched' : 'BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched/afterDiMuonPairBgEstZmumuMuonMisIdEnriched/',
    'WplusJetsEnriched' : 'BgEstTemplateAnalyzer_WplusJetsEnriched/afterDiMuonVetoBgEstWplusJetsEnriched/',
    'TTplusJetsEnriched' : 'BgEstTemplateAnalyzer_TTplusJetsEnriched/afterJetEt60BgEstTTplusJetsEnriched/',
    'QCDenriched' : 'BgEstTemplateAnalyzer_QCDenriched/afterDiMuonVetoBgEstQCDenriched/'
}

pureProcessBgEnrichedSelections = {
    'ZmumuJetMisIdEnriched' : 'Zmumu',
    'ZmumuMuonMisIdEnriched' : 'Zmumu',
    'WplusJetsEnriched' : 'WplusJets',
    'TTplusJetsEnriched' : 'TTplusJets',
    'QCDenriched' : 'QCD',
}

meName_visMass = "DiTauCandidateQuantities/VisMass"
meName_visMass_norm = "DiTauCandidateQuantities/VisMassShape"

meName_SVfitMass = "DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass"
meName_SVfitMass_norm = "DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/MassShape"

meName_pfMEt = "PFMEtQuantities/MEtPt"
meName_pfMEt_norm = "PFMEtQuantities/MEtPtShape"

meName_visPt = "DiTauCandidateQuantities/VisPt"
meName_visPt_norm = "DiTauCandidateQuantities/VisPtShape"

rebinningBgEnrichedSelections = {
    'ZmumuJetMisIdEnriched' : {
        'visMass' : 5,
        'SVfitMass' : 5,
        'pfMEt' : 3,
	'visPt' : 2
    },
    'ZmumuMuonMisIdEnriched' : {
        'visMass' : 1,
        'SVfitMass' : 1,
        'pfMEt' : 3,
	'visPt' : 2
    },
    'WplusJetsEnriched' : {
        'visMass' : 2,
        'SVfitMass' : 2,
        'pfMEt' : 3,
	'visPt' : 2
    },
    'TTplusJetsEnriched' : {
        'visMass' : 5,
        'SVfitMass' : 5,
        'pfMEt' : 5,
    	'visPt' : 5	
    },
    'QCDenriched' : {
        'visMass' : 2,
        'SVfitMass' : 2,
        'pfMEt' : 3,
	'visPt' : 2
    }
}    

#--------------------------------------------------------------------------------
# load template histogram for Z --> tau+ tau- signal events,
# produced by MCEmbeddingTools from Z --> mu+ mu- events selected in (pseudo)data
#--------------------------------------------------------------------------------

process.loadTemplateHistZtoMuTau_Ztautau = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = cms.PSet(
        inputFileNames = cms.vstring(
            'file:/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/2010Nov14/Ztautau_templates_from_ZmumuEmbedding.root'
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('/analysis/harvested/ZtoMuTau_from_ZmumuEmbedding' + '/' + dqmDirectoryAnalysis)
    )
)

#--------------------------------------------------------------------------------
# load template histogram of for different types of background events,
# obtained from background enriched control samples
#--------------------------------------------------------------------------------

process.loadTemplateHistZtoMuTau = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = cms.PSet(
        inputFileNames = cms.vstring(
            ##getHarvestingFilePath('ZtoMuTau_bgEstTemplate') + '/' + 'plotsZtoMuTau_bgEstTemplate_all.root'
            '/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/2010Dec17/plotsZtoMuTau_bgEstTemplate_all.root'
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('/template')
    )
)

#--------------------------------------------------------------------------------
# load distributions expected for different signal/background processes
# and observed in (pseudo)data in events passing final analysis selection criteria
#--------------------------------------------------------------------------------

process.loadAnalysisHistZtoMuTau = cms.EDAnalyzer("DQMFileLoader",
    data = cms.PSet(
        inputFileNames = cms.vstring(
            #getHarvestingFilePath('ZtoMuTau') + '/' + 'plotsZtoMuTau_all.root'
            ##getHarvestingFilePath('ZtoMuTau_bgEstTemplate') + '/' + 'plotsZtoMuTau_all.root'
            '/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau/2010Dec14ii/plotsZtoMuTau_all.root'
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('/analysis')
    )
)

#--------------------------------------------------------------------------------
# combine two bins into one,
# in order to mitigate effect of small event statistics
#--------------------------------------------------------------------------------

meName_visMass_rebinned = meName_visMass + 'Rebinned'
meName_SVfitMass_rebinned = meName_SVfitMass + 'Rebinned'

meName_pfMEt_rebinned = meName_pfMEt + 'Rebinned'

meName_visPt_rebinned = meName_visPt + 'Rebinned'

rebinningSequence = None

for processName, dqmDirectoryProcess in dqmDirectoriesProcess.items():
    if processName != 'Ztautau_from_ZmumuEmbedding':
        for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
            visMassRebinningModule = cms.EDAnalyzer("DQMHistRebinner",
                config = cms.VPSet(cms.PSet(
                    meName_original = cms.string(
                        '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                       + dqmDirectoryBgEnrichedSelection + meName_visMass
                    ),
                    meName_rebinned = cms.string(
                        '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                       + dqmDirectoryBgEnrichedSelection + meName_visMass_rebinned
                    )
                )),
                binning = cms.PSet(
                    x = cms.PSet(
                        combineBins = cms.uint32(rebinningBgEnrichedSelections[bgEnrichedSelectionName]['visMass'])
                    )
                )                   
            )
            visMassRebinningModuleName = "rebinTemplateHistVisMassMassZtoMuTau%s%s" % (bgEnrichedSelectionName, processName)
            setattr(process, visMassRebinningModuleName, visMassRebinningModule)
            if rebinningSequence is None:
                rebinningSequence = cms.Sequence(visMassRebinningModule)
            else:
                rebinningSequence._seq = rebinningSequence._seq * visMassRebinningModule
            SVfitMassRebinningModule = cms.EDAnalyzer("DQMHistRebinner",
                config = cms.VPSet(cms.PSet(
                    meName_original = cms.string(
                        '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                       + dqmDirectoryBgEnrichedSelection + meName_SVfitMass
                    ),
                    meName_rebinned = cms.string(
                        '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                       + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_rebinned
                    )
                )),
                binning = cms.PSet(
                    x = cms.PSet(
                        combineBins = cms.uint32(rebinningBgEnrichedSelections[bgEnrichedSelectionName]['SVfitMass'])
                    )
                )                   
            )
            SVfitMassRebinningModuleName = "rebinTemplateHistSVfitMassZtoMuTau%s%s" % (bgEnrichedSelectionName, processName)
            setattr(process, SVfitMassRebinningModuleName, SVfitMassRebinningModule)
            rebinningSequence._seq = rebinningSequence._seq * SVfitMassRebinningModule

            pfMEtRebinningModule = cms.EDAnalyzer("DQMHistRebinner",
                config = cms.VPSet(cms.PSet(
                    meName_original = cms.string(
                        '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                       + dqmDirectoryBgEnrichedSelection + meName_pfMEt
                    ),
                    meName_rebinned = cms.string(
                        '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                       + dqmDirectoryBgEnrichedSelection + meName_pfMEt_rebinned
                    )
                )),
                binning = cms.PSet(
                    x = cms.PSet(
                        combineBins = cms.uint32(rebinningBgEnrichedSelections[bgEnrichedSelectionName]['pfMEt'])
                    )
                )                   
            )
            pfMEtRebinningModuleName = "rebinTemplateHistPFMEtZtoMuTau%s%s" % (bgEnrichedSelectionName, processName)
            setattr(process, pfMEtRebinningModuleName, pfMEtRebinningModule)
            rebinningSequence._seq = rebinningSequence._seq * pfMEtRebinningModule

	    visPtRebinningModule = cms.EDAnalyzer("DQMHistRebinner",
                config = cms.VPSet(cms.PSet(
                    meName_original = cms.string(
                        '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                       + dqmDirectoryBgEnrichedSelection + meName_visPt
                    ),
                    meName_rebinned = cms.string(
                        '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                       + dqmDirectoryBgEnrichedSelection + meName_visPt_rebinned
                    )
                )),
                binning = cms.PSet(
                    x = cms.PSet(
                        combineBins = cms.uint32(rebinningBgEnrichedSelections[bgEnrichedSelectionName]['visPt'])
                    )
                )                   
            )
            visPtRebinningModuleName = "rebinTemplateHistVisPtZtoMuTau%s%s" % (bgEnrichedSelectionName, processName)
            setattr(process, visPtRebinningModuleName, visPtRebinningModule)
            rebinningSequence._seq = rebinningSequence._seq * visPtRebinningModule

    visMassRebinningModule = cms.EDAnalyzer("DQMHistRebinner",
        config = cms.VPSet(cms.PSet(
            meName_original = cms.string(
                '/analysis/harvested/' + dqmDirectoryProcess['analysis'] + '/' \
               + dqmDirectoryAnalysis + meName_visMass
            ),
            meName_rebinned = cms.string(
                '/analysis/harvested/' + dqmDirectoryProcess['analysis'] + '/' \
               + dqmDirectoryAnalysis + meName_visMass_rebinned
            )
        )),
        binning = cms.PSet(
            x = cms.PSet(
                combineBins = cms.uint32(rebinningAnalysis['visMass'])
            )
        )
    )
    visMassRebinningModuleName = "rebinAnalysisHistVisMassZtoMuTau%s" % processName
    setattr(process, visMassRebinningModuleName, visMassRebinningModule)
    rebinningSequence._seq = rebinningSequence._seq * visMassRebinningModule
    SVfitMassRebinningModule = cms.EDAnalyzer("DQMHistRebinner",
        config = cms.VPSet(cms.PSet(
            meName_original = cms.string(
                '/analysis/harvested/' + dqmDirectoryProcess['analysis'] + '/' \
               + dqmDirectoryAnalysis + meName_SVfitMass
            ),
            meName_rebinned = cms.string(
                '/analysis/harvested/' + dqmDirectoryProcess['analysis'] + '/' \
               + dqmDirectoryAnalysis + meName_SVfitMass_rebinned
            )
        )),
        binning = cms.PSet(
            x = cms.PSet(
                combineBins = cms.uint32(rebinningAnalysis['SVfitMass'])
            )
        )                   
    )
    SVfitMassRebinningModuleName = "rebinAnalysisHistSVfitMassZtoMuTau%s" % processName
    setattr(process, SVfitMassRebinningModuleName, SVfitMassRebinningModule)
    rebinningSequence._seq = rebinningSequence._seq * SVfitMassRebinningModule

setattr(process, "rebinHistZtoMuTau", rebinningSequence)

#--------------------------------------------------------------------------------
# correct for "bias" of W + jets template histograms
# caused by cuts on Mt(muon + tau-jet) transverse mass and CDF (Pzeta - 1.5*PzetaVis) variable
# (cf. CMS AN-2010/088)
#--------------------------------------------------------------------------------

meName_visMass_corrected = meName_visMass + 'Corrected'
meName_SVfitMass_corrected = meName_SVfitMass + 'Corrected'

process.correctTemplateHistZtoMuTau = cms.EDAnalyzer("DQMHistBiasCorrection",
    config = cms.VPSet(
        cms.PSet(
            meName_uncorrected = cms.string(
                '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
               + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_visMass_rebinned
            ),
            meName_corrected = cms.string(
                '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
               + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_visMass_corrected
            ),
            meName_corrNumerator = cms.string(
                '/analysis/harvested/' + dqmDirectoriesProcess['WplusJets']['analysis'] + '/' \
               + dqmDirectoryAnalysis + meName_visMass_rebinned
            ),
            meName_corrDenominator = cms.string(
                '/template/harvested/' + dqmDirectoriesProcess['WplusJets']['template'] + '/' \
               + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_visMass_rebinned
            )
        ),
        cms.PSet(
            meName_uncorrected = cms.string(
                '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
               + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_SVfitMass_rebinned
            ),
            meName_corrected = cms.string(
                '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
               + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_SVfitMass_corrected
            ),
            meName_corrNumerator = cms.string(
                '/analysis/harvested/' + dqmDirectoriesProcess['WplusJets']['analysis'] + '/' \
               + dqmDirectoryAnalysis + meName_SVfitMass_rebinned
            ),
            meName_corrDenominator = cms.string(
                '/template/harvested/' + dqmDirectoriesProcess['WplusJets']['template'] + '/' \
               + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_SVfitMass_rebinned
            )
        )
    )
)                                                     

#--------------------------------------------------------------------------------
# normalize to unit area distribution of visible muon + tau-jet mass for Z --> tau+ tau- signal
# and different types of background events
#--------------------------------------------------------------------------------

meName_visMass_corrected_norm = meName_visMass + 'CorrectedShape'
meName_SVfitMass_corrected_norm = meName_SVfitMass + 'CorrectedShape'

jobsHistNormalization = []

for processName, dqmDirectoryProcess in dqmDirectoriesProcess.items():
    if processName != 'Ztautau_from_ZmumuEmbedding':
        for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
            jobsHistNormalization.append(cms.PSet(
                meName_input = cms.string(
                    '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                   + dqmDirectoryBgEnrichedSelection + meName_visMass_rebinned
                ),
                meName_output = cms.string(
                    '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                   + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
                )
            ))
            jobsHistNormalization.append(cms.PSet(
                meName_input = cms.string(
                    '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                   + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_rebinned
                ),
                meName_output = cms.string(
                    '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                   + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
                )
            ))

            jobsHistNormalization.append(cms.PSet(
                meName_input = cms.string(
                    '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                   + dqmDirectoryBgEnrichedSelection + meName_pfMEt_rebinned
                ),
                meName_output = cms.string(
                    '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                   + dqmDirectoryBgEnrichedSelection + meName_pfMEt_norm
                )
            ))

            jobsHistNormalization.append(cms.PSet(
                meName_input = cms.string(
                '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                   + dqmDirectoryBgEnrichedSelection + meName_visPt_rebinned
                ),
                meName_output = cms.string(
                    '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
                   + dqmDirectoryBgEnrichedSelection + meName_visPt_norm
                )
            ))

    jobsHistNormalization.append(cms.PSet(
        meName_input = cms.string(
            '/analysis/harvested/' + dqmDirectoryProcess['analysis'] + '/' \
           + dqmDirectoryAnalysis + meName_visMass_rebinned
        ),
        meName_output = cms.string(
            '/analysis/harvested/' + dqmDirectoryProcess['analysis'] + '/' \
           + dqmDirectoryAnalysis + meName_visMass_norm
        )
    ))
    jobsHistNormalization.append(cms.PSet(
        meName_input = cms.string(
            '/analysis/harvested/' + dqmDirectoryProcess['analysis'] + '/' \
           + dqmDirectoryAnalysis + meName_SVfitMass_rebinned
        ),
        meName_output = cms.string(
            '/analysis/harvested/' + dqmDirectoryProcess['analysis'] + '/' \
           + dqmDirectoryAnalysis + meName_SVfitMass_norm
        )
    ))
jobsHistNormalization.append(cms.PSet(
    meName_input = cms.string(
        '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
       + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_visMass_corrected
    ),
    meName_output = cms.string(
        '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
       + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_visMass_corrected_norm
    )
))
jobsHistNormalization.append(cms.PSet(
    meName_input = cms.string(
        '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
       + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_SVfitMass_corrected
    ),
    meName_output = cms.string(
        '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
       + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_SVfitMass_corrected_norm
    )
)) 
    
process.normalizeHistZtoMuTau = cms.EDAnalyzer("DQMHistNormalizer",
    config = cms.VPSet(jobsHistNormalization),
    norm = cms.double(1.)
)

#--------------------------------------------------------------------------------
# produce cumulative distributions
#--------------------------------------------------------------------------------

meName_visMass_integrated = meName_visMass + 'IntegratedShape'
meName_SVfitMass_integrated = meName_SVfitMass + 'IntegratedShape'

meName_pfMEt_integrated = meName_pfMEt + 'IntegratedShape'

meName_visPt_integrated = meName_visPt + 'IntegratedShape'

jobsHistIntegration = []

for processName, dqmDirectoryProcess in dqmDirectoriesProcess.items():
    for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
        jobsHistIntegration.append(cms.PSet(
            meName_input = cms.string(
                '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
               + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
            ),
            meName_output = cms.string(
                '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
               + dqmDirectoryBgEnrichedSelection + meName_visMass_integrated
            ),
            integrateFrom = cms.string("right")
        ))
        jobsHistIntegration.append(cms.PSet(
            meName_input = cms.string(
                '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
               + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
            ),
            meName_output = cms.string(
                '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
               + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_integrated
            ),
            integrateFrom = cms.string("right")
        ))

        jobsHistIntegration.append(cms.PSet(
            meName_input = cms.string(
                '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
               + dqmDirectoryBgEnrichedSelection + meName_pfMEt_norm
            ),
            meName_output = cms.string(
                '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
               + dqmDirectoryBgEnrichedSelection + meName_pfMEt_integrated
            ),
            integrateFrom = cms.string("right")
        ))

        jobsHistIntegration.append(cms.PSet(
            meName_input = cms.string(
                '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
               + dqmDirectoryBgEnrichedSelection + meName_visPt_norm
            ),
            meName_output = cms.string(
                '/template/harvested/' + dqmDirectoryProcess['template'] + '/' \
               + dqmDirectoryBgEnrichedSelection + meName_visPt_integrated
            ),
            integrateFrom = cms.string("right")
        ))
    
process.integrateHistZtoMuTau = cms.EDAnalyzer("DQMHistIntegrator",
    config = cms.VPSet(jobsHistIntegration)
)

#--------------------------------------------------------------------------------
# compute Kolmogorov-Smirnov probabilities for distributions observed in Data
# to agree with Monte Carlo predictions in the background enriched regions
#--------------------------------------------------------------------------------

meName_visMass_compatibility = meName_visMass + 'Compatibility'
meName_SVfitMass_compatibility = meName_SVfitMass + 'Compatibility'

meName_pfMEt_compatibility = meName_pfMEt + 'Compatibility'

meName_visPt_compatibility = meName_visPt + 'Compatibility'

jobsKolmogorovTest = []

for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
    jobsKolmogorovTest.append(cms.PSet(
        meName_test = cms.string(
            '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
        ),
        meName_reference = cms.string(
            '/template/harvested/' + dqmDirectoriesProcess['smSum']['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
        ),
        meName_compatibility = cms.string(
            '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visMass_compatibility
        )
    ))
    jobsKolmogorovTest.append(cms.PSet(
        meName_test = cms.string(
            '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
        ),
        meName_reference = cms.string(
            '/template/harvested/' + dqmDirectoriesProcess['smSum']['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
        ),
        meName_compatibility = cms.string(
            '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_compatibility
        )
    ))

    jobsKolmogorovTest.append(cms.PSet(
        meName_test = cms.string(
            '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_pfMEt_norm
        ),
        meName_reference = cms.string(
            '/template/harvested/' + dqmDirectoriesProcess['smSum']['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_pfMEt_norm
        ),
        meName_compatibility = cms.string(
            '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_pfMEt_compatibility
        )
    ))

    jobsKolmogorovTest.append(cms.PSet(
        meName_test = cms.string(
            '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visPt_norm
        ),
        meName_reference = cms.string(
            '/template/harvested/' + dqmDirectoriesProcess['smSum']['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visPt_norm
        ),
        meName_compatibility = cms.string(
            '/template/harvested/' + dqmDirectoriesProcess['Data']['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visPt_compatibility
        )
    ))
    
process.compKolmogorovProbZtoMuTau = cms.EDAnalyzer("DQMHistKolmogorovTest",
    config = cms.VPSet(jobsKolmogorovTest)
)

#--------------------------------------------------------------------------------
# plot template histograms obtained from Monte Carlo
# compared to the shapes determined by background enriched regions in (pseudo)Data
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

drawTemplateHistConfiguratorZtoMuTau = drawTemplateHistConfigurator(
    template = drawJobTemplateHist
)

def configurePlotZtoMuTauIntegrated(dqmDirectoryData, dqmDirectoryBgEstMC_smSum, dqmDirectoryBgEnrichedSelection,
                                    meName_integrated, meName_compatibility,
                                    plotName, plotTitle):
    drawTemplateHistConfiguratorZtoMuTauIntegrated = drawTemplateHistConfigurator(
        template = drawJobTemplateHistIntegrated.clone(
            labels = cms.vstring('ksProb')
        )
    )    
    drawTemplateHistConfiguratorZtoMuTauIntegrated.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_integrated,
            '/template/harvested/' + dqmDirectoryData['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_integrated
        ],
        name = ("%s_%s" % (bgEnrichedSelectionName, plotName)),
        title = ("%s: %s" % (bgEnrichedSelectionName, plotTitle))
    )
    plotHistZtoMuTauIntegrated = plotHistZtoMuTau.clone(
        labels = cms.PSet(
            ksProb = label_mcNormScale.clone(
                text = cms.vstring('KS prob.: %f1.2'),
                meName = cms.string(
                    '/template/harvested/' + dqmDirectoryData['template'] + '/' \
                   + dqmDirectoryBgEnrichedSelection + meName_compatibility
                )
            )
        ),
        drawJobs = drawTemplateHistConfiguratorZtoMuTauIntegrated.configure(),
        indOutputFileName = cms.string('plotBgEstTemplateZtoMuTauIntegrated_#PLOT#.pdf')
    )

    return plotHistZtoMuTauIntegrated

drawAnalysisHistConfiguratorZtoMuTauData = drawTemplateHistConfigurator(
    template = drawJobAnalysisHistData
)

drawAnalysisHistConfiguratorZtoMuTauMC = drawTemplateHistConfigurator(
    template = drawJobAnalysisHistMC
)

drawAnalysisHistConfiguratorZtoMuTauZmumuEmbedding = drawTemplateHistConfigurator(
    template = drawJobAnalysisHistZmumuEmbedding
)

plotTemplateHistZtoMuTauIntegratedSequence = None

for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
    dqmDirectoryData = dqmDirectoriesProcess['Data']
    dqmDirectoryBgEstMC_pure = dqmDirectoriesProcess[pureProcessBgEnrichedSelections[bgEnrichedSelectionName]]
    dqmDirectoryBgEstMC_smSum = dqmDirectoriesProcess['smSum']
    dqmDirectoryAnalysisMC_pure = dqmDirectoriesProcess[pureProcessBgEnrichedSelections[bgEnrichedSelectionName]]

    drawTemplateHistConfiguratorZtoMuTau.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visMass_norm,
            '/template/harvested/' + dqmDirectoryBgEstMC_pure['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visMass_norm,
            '/template/harvested/' + dqmDirectoryData['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
        ],
        name = ("%s_visMass" % bgEnrichedSelectionName),
        title = ("%s: M_{vis}(Muon + Tau)" % bgEnrichedSelectionName)
    )
    drawTemplateHistConfiguratorZtoMuTau.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm,
            '/template/harvested/' + dqmDirectoryBgEstMC_pure['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm,
            '/template/harvested/' + dqmDirectoryData['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
        ],
        name = ("%s_SVfitMass" % bgEnrichedSelectionName),
        title = ("%s: M(Muon + Tau), SVfit method" % bgEnrichedSelectionName)
    )

    drawTemplateHistConfiguratorZtoMuTau.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_pfMEt_norm,
            '/template/harvested/' + dqmDirectoryBgEstMC_pure['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_pfMEt_norm,
            '/template/harvested/' + dqmDirectoryData['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_pfMEt_norm
        ],
        name = ("%s_pfMEt" % bgEnrichedSelectionName),
        title = ("%s: Particle-Flow MEt" % bgEnrichedSelectionName)
    )	

    drawTemplateHistConfiguratorZtoMuTau.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visPt_norm,
            '/template/harvested/' + dqmDirectoryBgEstMC_pure['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visPt_norm,
            '/template/harvested/' + dqmDirectoryData['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visPt_norm
        ],
        name = ("%s_visPt" % bgEnrichedSelectionName),
        title = ("%s: P_{T}(Muon + Tau)" % bgEnrichedSelectionName)
    )

    plotHistZtoMuTauIntegrated_visMass = configurePlotZtoMuTauIntegrated(
        dqmDirectoryData, dqmDirectoryBgEstMC_smSum, dqmDirectoryBgEnrichedSelection,
        meName_visMass_integrated, meName_visMass_compatibility, "visMass", "M_{vis}(Muon + Tau)")
    plotHistZtoMuTauIntegratedName_visMass = "plotHistZtoMuTauIntegrated%sVisMass" % bgEnrichedSelectionName
    setattr(process, plotHistZtoMuTauIntegratedName_visMass, plotHistZtoMuTauIntegrated_visMass)
    if plotTemplateHistZtoMuTauIntegratedSequence is None:
        plotTemplateHistZtoMuTauIntegratedSequence = cms.Sequence(plotHistZtoMuTauIntegrated_visMass)
    else:
        plotTemplateHistZtoMuTauIntegratedSequence += plotHistZtoMuTauIntegrated_visMass
    plotHistZtoMuTauIntegrated_SVfitMass = configurePlotZtoMuTauIntegrated(
        dqmDirectoryData, dqmDirectoryBgEstMC_smSum, dqmDirectoryBgEnrichedSelection,
        meName_SVfitMass_integrated, meName_SVfitMass_compatibility, "SVfitMass", "M(Muon + Tau), SVfit method")
    plotHistZtoMuTauIntegratedName_SVfitMass = "plotHistZtoMuTauIntegrated%sSVfitMass" % bgEnrichedSelectionName
    setattr(process, plotHistZtoMuTauIntegratedName_SVfitMass, plotHistZtoMuTauIntegrated_SVfitMass)
    plotTemplateHistZtoMuTauIntegratedSequence += plotHistZtoMuTauIntegrated_SVfitMass

    plotHistZtoMuTauIntegrated_pfMEt = configurePlotZtoMuTauIntegrated(
        dqmDirectoryData, dqmDirectoryBgEstMC_smSum, dqmDirectoryBgEnrichedSelection,
        meName_pfMEt_integrated, meName_pfMEt_compatibility, "pfMEt", "Particle-Flow MEt")
    plotHistZtoMuTauIntegratedName_pfMEt = "plotHistZtoMuTauIntegrated%sPFMEt" % bgEnrichedSelectionName
    setattr(process, plotHistZtoMuTauIntegratedName_pfMEt, plotHistZtoMuTauIntegrated_pfMEt)
    plotTemplateHistZtoMuTauIntegratedSequence += plotHistZtoMuTauIntegrated_pfMEt

    plotHistZtoMuTauIntegrated_visPt = configurePlotZtoMuTauIntegrated(
        dqmDirectoryData, dqmDirectoryBgEstMC_smSum, dqmDirectoryBgEnrichedSelection,
        meName_visPt_integrated, meName_visPt_compatibility, "visPt", "P_{T}(Muon + Tau)")
    plotHistZtoMuTauIntegratedName_visPt = "plotHistZtoMuTauIntegrated%sVisPt" % bgEnrichedSelectionName
    setattr(process, plotHistZtoMuTauIntegratedName_visPt, plotHistZtoMuTauIntegrated_visPt)
    plotTemplateHistZtoMuTauIntegratedSequence += plotHistZtoMuTauIntegrated_visPt

    drawAnalysisHistConfiguratorZtoMuTauData.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryData['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visMass_norm,
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure['analysis'] + '/' \
           + dqmDirectoryAnalysis + meName_visMass_norm
        ],
        name = ("%s_visMass" % bgEnrichedSelectionName),
        title = ("%s: M_{vis}(Muon + Tau)" % bgEnrichedSelectionName)
    )
    drawAnalysisHistConfiguratorZtoMuTauData.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryData['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm,
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure['analysis'] + '/' \
           + dqmDirectoryAnalysis + meName_SVfitMass_norm
        ],
        name = ("%s_SVfitMass" % bgEnrichedSelectionName),
        title = ("%s: M(Muon + Tau), SVfit method" % bgEnrichedSelectionName)
    )

    drawAnalysisHistConfiguratorZtoMuTauMC.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_visMass_norm,
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure['analysis'] + '/' \
           + dqmDirectoryAnalysis + meName_visMass_norm
        ],
        name = ("%s_visMass" % bgEnrichedSelectionName),
        title = ("%s: M_{vis}(Muon + Tau)" % bgEnrichedSelectionName)
    )
    drawAnalysisHistConfiguratorZtoMuTauMC.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum['template'] + '/' \
           + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm,
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure['analysis'] + '/' \
           + dqmDirectoryAnalysis + meName_SVfitMass_norm
        ],
        name = ("%s_SVfitMass" % bgEnrichedSelectionName),
        title = ("%s: M(Muon + Tau), SVfit method" % bgEnrichedSelectionName)
    )

process.plotTemplateHistZtoMuTauIntegrated = plotTemplateHistZtoMuTauIntegratedSequence

drawAnalysisHistConfiguratorZtoMuTauData.add(
    meNames = [
        '/template/harvested/' + dqmDirectoryData['template'] + '/' \
       + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_visMass_corrected_norm,
        '/analysis/harvested/' + dqmDirectoriesProcess['WplusJets']['analysis'] + '/' \
       + dqmDirectoryAnalysis + meName_visMass_norm
    ],
    name = ("%s_visMass_corrected" % bgEnrichedSelectionName),
    title = ("%s: M_{vis}(Muon + Tau)" % bgEnrichedSelectionName)
)
drawAnalysisHistConfiguratorZtoMuTauData.add(
    meNames = [
        '/template/harvested/' + dqmDirectoryData['template'] + '/' \
       + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_SVfitMass_corrected_norm,
        '/analysis/harvested/' + dqmDirectoriesProcess['WplusJets']['analysis'] + '/' \
       + dqmDirectoryAnalysis + meName_SVfitMass_norm
    ],
    name = ("%s_SVfitMass_corrected" % bgEnrichedSelectionName),
    title = ("%s: M(Muon + Tau), SVfit method" % bgEnrichedSelectionName)
)    

drawAnalysisHistConfiguratorZtoMuTauZmumuEmbedding.add(
    meNames = [
        '/analysis/harvested/' + dqmDirectoriesProcess['Ztautau']['analysis'] + '/' \
       + dqmDirectoryAnalysis + meName_visMass_norm,
        '/analysis/harvested/' + dqmDirectoriesProcess['Ztautau_from_ZmumuEmbedding']['analysis'] + '/' \
       + dqmDirectoryAnalysis + meName_visMass_norm
    ],
    name = ("ZmumuEmbedding_visMass"),
    title = ("ZmumuEmbedding: M_{vis}(Muon + Tau)")
)
drawAnalysisHistConfiguratorZtoMuTauZmumuEmbedding.add(
    meNames = [
        '/analysis/harvested/' + dqmDirectoriesProcess['Ztautau']['analysis'] + '/' \
       + dqmDirectoryAnalysis + meName_SVfitMass_norm,
        '/analysis/harvested/' + dqmDirectoriesProcess['Ztautau_from_ZmumuEmbedding']['analysis'] + '/' \
       + dqmDirectoryAnalysis + meName_SVfitMass_norm
    ],
    name = ("ZmumuEmbedding_SVfitMass"),
    title = ("ZmumuEmbedding: M(Muon + Tau), SVfit method")
)    

process.plotTemplateHistZtoMuTau = plotHistZtoMuTau.clone(
    drawJobs = drawTemplateHistConfiguratorZtoMuTau.configure(),
    indOutputFileName = cms.string('plotBgEstTemplateZtoMuTau_#PLOT#.pdf')
)

process.plotAnalysisHistZtoMuTauData = plotHistZtoMuTau.clone(
    drawJobs = drawAnalysisHistConfiguratorZtoMuTauData.configure(),
    indOutputFileName = cms.string('plotBgEstTemplateData_vs_AnalysisZtoMuTau_#PLOT#.pdf')
)

process.plotAnalysisHistZtoMuTauMC = plotHistZtoMuTau.clone(
    drawJobs = drawAnalysisHistConfiguratorZtoMuTauMC.configure(),
    indOutputFileName = cms.string('plotBgEstTemplateMC_vs_AnalysisZtoMuTau_#PLOT#.pdf')
)

process.plotAnalysisHistZtoMuTauZmumuEmbedding = plotHistZtoMuTau.clone(
    drawJobs = drawAnalysisHistConfiguratorZtoMuTauZmumuEmbedding.configure(),
    indOutputFileName = cms.string('plotZmumuEmbedding_vs_AnalysisZtoMuTau_#PLOT#.pdf')
)

plotAnalysisHistZtoMuTauStacked = None

plotZtoMuTauStacked_template = plots_ZtoMuTau.clone(
    plots = cms.PSet(
        dqmMonitorElements = cms.vstring(''),
        processes = cms.vstring(
            'TTplusJets',
            'Zmumu',
            'WplusJets',
            'QCD',
            'Ztautau',
            'Data'
        )
    ),
    stack = cms.vstring(
        'TTplusJets',
	'Zmumu',
        'WplusJets',
        'QCD',
        'Ztautau'
    )
)

for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():

    drawJobConfiguratorZtoMuTauStacked = drawJobConfigurator(
        template = plotZtoMuTauStacked_template,
        dqmDirectory = '#PROCESSDIR#' + '/' + dqmDirectoryBgEnrichedSelection
    )

    drawJobConfiguratorZtoMuTauStacked.add(
        plots = [
	    drawJobConfigEntry(
                meName = meName_visMass_rebinned,
                title = "M_{vis}(Muon + Tau)",
                xAxis = 'Mass',
                name = "mVisible"
            ),
            drawJobConfigEntry(
                meName = meName_SVfitMass_rebinned,
                title = "M(Muon + Tau), SVfit method",
                xAxis = 'Mass',
                name = "mSVmethod"
            ),
            drawJobConfigEntry(
                meName = meName_pfMEt_rebinned,
                title = "PFMEt",
                xAxis = 'Pt',
                name = "pfMet"
            ),
            drawJobConfigEntry(
                meName = meName_visPt_rebinned,
                title = "P_{T}(Muon + Tau)",
                xAxis = 'Pt',
                name = "visPt"
            )
	]
    )

    dqmHistPlotterModule = dqmHistPlotter_template.clone(
	processes = cms.PSet(
            Ztautau = cms.PSet(
                dqmDirectory = cms.string('/template/harvested/' + dqmDirectoriesProcess['Ztautau']['template']),
                legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['Ztautau']['legendEntry']),
                type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['Ztautau']['type'])
            ),
            Zmumu = cms.PSet(
                dqmDirectory = cms.string('/template/harvested/' + dqmDirectoriesProcess['Zmumu']['template']),
                legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['Zmumu']['legendEntry']),
                type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['Zmumu']['type'])
            ),
            WplusJets = cms.PSet(
                dqmDirectory = cms.string('/template/harvested/' + dqmDirectoriesProcess['WplusJets']['template']),
                legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['WplusJetsSum']['legendEntry']),
                type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['WplusJetsSum']['type'])
            ),
            TTplusJets = cms.PSet(
                dqmDirectory = cms.string('/template/harvested/' + dqmDirectoriesProcess['TTplusJets']['template']),
                legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['TTplusJets']['legendEntry']),
                type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['TTplusJets']['type'])
            ),
            QCD = cms.PSet(
                dqmDirectory = cms.string('/template/harvested/' + dqmDirectoriesProcess['QCD']['template']),
                legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['qcdSum']['legendEntry']),
                type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['qcdSum']['type'])
            ),
            Data = cms.PSet(
                dqmDirectory = cms.string('/template/harvested/' + dqmDirectoriesProcess['Data']['template']),
                legendEntry = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['data']['legendEntry']),
                type = cms.string(recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['data']['type'])
            )
        ),
        legends = cms.PSet(
            regular = copy.deepcopy(styles.legend_regular)
        ),
        labels = cms.PSet(
            mcNormScale = dqmHistPlotter_template.labels.mcNormScale.clone(
                posY = cms.double(0.74),
                sizeY = cms.double(0.14),
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
                Ztautau = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['Ztautau']['drawOption'],
                Zmumu = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['Zmumu']['drawOption'],
                WplusJets = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['WplusJetsSum']['drawOption'],
                TTplusJets = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['TTplusJets']['drawOption'],
                QCD = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['qcdSum']['drawOption'],
                Data = recoSampleDefinitionsZtoMuTau_7TeV['ALL_SAMPLES']['data']['drawOption']
            )
        ),
        drawJobs = drawJobConfiguratorZtoMuTauStacked.configure(),
        outputFilePath = cms.string('./plots/'),
        indOutputFileName = cms.string('bgEstControlZtoMuTau_%s_#PLOT#.pdf' % bgEnrichedSelectionName)
    )
    dqmHistPlotterModuleName = "plotZtoMuTauStacked%s" % bgEnrichedSelectionName
    setattr(process, dqmHistPlotterModuleName, dqmHistPlotterModule)

    if plotAnalysisHistZtoMuTauStacked is None:
	plotAnalysisHistZtoMuTauStacked = cms.Sequence(dqmHistPlotterModule)
    else:
	plotAnalysisHistZtoMuTauStacked += dqmHistPlotterModule

process.plotAnalysisHistZtoMuTauStacked = plotAnalysisHistZtoMuTauStacked
    
process.saveBgEstTemplateHistZtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string(
        ##getHarvestingFilePath('ZtoMuTau_bgEstTemplate') + '/' + 'bgEstTemplateHistZtoMuTau_skimmed.root'
        '/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/2010Dec17/bgEstTemplateHistZtoMuTau_skimmed.root'
    ),
    outputCommands = cms.vstring(
        'drop *',
        'keep /template/harvested/ZtautauSum/*',
        'keep /template/harvested/Ztautau_from_ZmumuEmbedding/*',
        'keep /template/harvested/Zmumu/*',
        'keep /template/harvested/qcdSum/*',
        'keep /template/harvested/WplusJets/*',
        'keep /template/harvested/WplusJetsSum/*',
        'keep /template/harvested/TTplusJets/*',
        'keep /template/harvested/data/*'
    )
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.p = cms.Path(
    process.loadTemplateHistZtoMuTau_Ztautau
   + process.loadTemplateHistZtoMuTau
   + process.loadAnalysisHistZtoMuTau
   + process.dumpDQMStore
   + process.rebinHistZtoMuTau
   + process.correctTemplateHistZtoMuTau
   + process.normalizeHistZtoMuTau
   + process.integrateHistZtoMuTau 
   + process.compKolmogorovProbZtoMuTau
   ##+ process.dumpDQMStore 
   + process.plotTemplateHistZtoMuTau
   + process.plotTemplateHistZtoMuTauIntegrated
   + process.plotAnalysisHistZtoMuTauData
   + process.plotAnalysisHistZtoMuTauMC
   + process.plotAnalysisHistZtoMuTauZmumuEmbedding    
   + process.plotAnalysisHistZtoMuTauStacked
   + process.saveBgEstTemplateHistZtoMuTau 
)

# print-out all python configuration parameter information
print process.dumpPython()


  
