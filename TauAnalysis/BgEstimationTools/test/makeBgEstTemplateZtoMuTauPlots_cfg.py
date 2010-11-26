import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *
from TauAnalysis.BgEstimationTools.templateHistDefinitions_cfi import \
  drawJobTemplateHist, drawJobAnalysisHistData, drawJobAnalysisHistMC, \
  plotBgEstData, plotBgEstMC_pure, plotBgEstMC_smSum, plotAnalysisMC_pure
from TauAnalysis.BgEstimationTools.tools.drawTemplateHistConfigurator import drawTemplateHistConfigurator
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, getJobId

process = cms.Process('makeBgEstTemplateZtoMuTauPlots')

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

rebinningAnalysis = {
    'visMass' : 2,
    'SVfitMass' : 2
}

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

rebinningBgEnrichedSelections = {
    'ZmumuJetMisIdEnriched' : {
        'visMass' : 4,
        'SVfitMass' : 5
    },
    'ZmumuMuonMisIdEnriched' : {
        'visMass' : 1,
        'SVfitMass' : 1
    },
    'WplusJetsEnriched' : {
        'visMass' : 2,
        'SVfitMass' : 2
    },
    ##'TTplusJetsEnriched' : {
    ##    'visMass' : 2,
    ##    'SVfitMass' : 2
    ##},
    'QCDenriched' : {
        'visMass' : 2,
        'SVfitMass' : 2
    }
}    

#--------------------------------------------------------------------------------
# load template histogram for Z --> tau+ tau- signal events,
# produced by MCEmbeddingTools from Z --> mu+ mu- events selected in (pseudo)data
#--------------------------------------------------------------------------------

process.loadTemplateHistZtoMuTau_Ztautau = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = cms.PSet(
        inputFileNames = cms.vstring(
            'file:/data1/veelken/bgEstTemplates/ZtoMuTau_from_selZmumu.root'
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('/template/ZtoMuTau_from_selZmumu')
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
            '/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/2010Nov14/plotsZtoMuTau_bgEstTemplate_all.root'
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
            '/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/2010Nov14/plotsZtoMuTau_all.root'
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

rebinningSequence = None

for processName, dqmDirectoryProcess in dqmDirectoriesProcess.items():
    for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
        visMassRebinningModule = cms.EDAnalyzer("DQMHistRebinner",
            config = cms.VPSet(cms.PSet(
                meName_original = cms.string(
                    '/template/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass
                ),
                meName_rebinned = cms.string(
                    '/template/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_rebinned
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
                    '/template/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass
                ),
                meName_rebinned = cms.string(
                    '/template/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_rebinned
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

    visMassRebinningModule = cms.EDAnalyzer("DQMHistRebinner",
        config = cms.VPSet(cms.PSet(
            meName_original = cms.string(
                '/analysis/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryAnalysis + meName_visMass
            ),
            meName_rebinned = cms.string(
                '/analysis/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryAnalysis + meName_visMass_rebinned
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
                '/analysis/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryAnalysis + meName_SVfitMass
            ),
            meName_rebinned = cms.string(
                '/analysis/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryAnalysis + meName_SVfitMass_rebinned
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
                '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
               + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_visMass_rebinned
            ),
            meName_corrected = cms.string(
                '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
               + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_visMass_corrected
            ),
            meName_corrNumerator = cms.string(
                '/analysis/harvested/' + dqmDirectoriesProcess['WplusJets'] + '/' \
               + dqmDirectoryAnalysis + meName_visMass_rebinned
            ),
            meName_corrDenominator = cms.string(
                '/template/harvested/' + dqmDirectoriesProcess['WplusJets'] + '/' \
               + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_visMass_rebinned
            )
        ),
        cms.PSet(
            meName_uncorrected = cms.string(
                '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
               + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_SVfitMass_rebinned
            ),
            meName_corrected = cms.string(
                '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
               + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_SVfitMass_corrected
            ),
            meName_corrNumerator = cms.string(
                '/analysis/harvested/' + dqmDirectoriesProcess['WplusJets'] + '/' \
               + dqmDirectoryAnalysis + meName_SVfitMass_rebinned
            ),
            meName_corrDenominator = cms.string(
                '/template/harvested/' + dqmDirectoriesProcess['WplusJets'] + '/' \
               + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_SVfitMass_rebinned
            )
        ),
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
    for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
        jobsHistNormalization.append(cms.PSet(
            meName_input = cms.string(
                '/template/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_rebinned
            ),
            meName_output = cms.string(
                '/template/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
            )
        ))
        jobsHistNormalization.append(cms.PSet(
            meName_input = cms.string(
                '/template/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_rebinned
            ),
            meName_output = cms.string(
                '/template/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
            )
        ))

    jobsHistNormalization.append(cms.PSet(
        meName_input = cms.string(
            '/analysis/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryAnalysis + meName_visMass_rebinned
        ),
        meName_output = cms.string(
            '/analysis/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryAnalysis + meName_visMass_norm
        )
    ))
    jobsHistNormalization.append(cms.PSet(
        meName_input = cms.string(
            '/analysis/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryAnalysis + meName_SVfitMass_rebinned
        ),
        meName_output = cms.string(
            '/analysis/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryAnalysis + meName_SVfitMass_norm
        )
    ))
jobsHistNormalization.append(cms.PSet(
    meName_input = cms.string(
        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
       + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_visMass_corrected
    ),
    meName_output = cms.string(
        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
       + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_visMass_corrected_norm
    )
))
jobsHistNormalization.append(cms.PSet(
    meName_input = cms.string(
        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
       + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_SVfitMass_corrected
    ),
    meName_output = cms.string(
        '/template/harvested/' + dqmDirectoriesProcess['Data'] + '/' \
       + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_SVfitMass_corrected_norm
    )
)) 
    
process.normalizeHistZtoMuTau = cms.EDAnalyzer("DQMHistNormalizer",
    config = cms.VPSet(jobsHistNormalization),
    norm = cms.double(1.)
)

#--------------------------------------------------------------------------------
# plot template histograms obtained from Monte Carlo
# compared to the shapes determined by background enriched regions in (pseudo)Data
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoMuTau = drawTemplateHistConfigurator(
    template = drawJobTemplateHist
)

drawAnalysisHistConfiguratorZtoMuTauData = drawTemplateHistConfigurator(
    template = drawJobAnalysisHistData
)

drawAnalysisHistConfiguratorZtoMuTauMC = drawTemplateHistConfigurator(
    template = drawJobAnalysisHistMC
)

for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
    dqmDirectoryData = dqmDirectoriesProcess['Data']
    dqmDirectoryBgEstMC_pure = dqmDirectoriesProcess[pureProcessBgEnrichedSelections[bgEnrichedSelectionName]]
    dqmDirectoryBgEstMC_smSum = dqmDirectoriesProcess['smSum']
    dqmDirectoryAnalysisMC_pure = dqmDirectoriesProcess[pureProcessBgEnrichedSelections[bgEnrichedSelectionName]]

    drawTemplateHistConfiguratorZtoMuTau.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm,
            '/template/harvested/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm,
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
        ],
        name = ("%s_visMass" % bgEnrichedSelectionName),
        title = ("%s: M_{vis}(Muon + Tau)" % bgEnrichedSelectionName)
    )
    drawTemplateHistConfiguratorZtoMuTau.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm,
            '/template/harvested/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm,
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
        ],
        name = ("%s_SVfitMass" % bgEnrichedSelectionName),
        title = ("%s: M(Muon + Tau), SVfit method" % bgEnrichedSelectionName)
    )

    drawAnalysisHistConfiguratorZtoMuTauData.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm,
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure + '/' + dqmDirectoryAnalysis + meName_visMass_norm
        ],
        name = ("%s_visMass" % bgEnrichedSelectionName),
        title = ("%s: M_{vis}(Muon + Tau)" % bgEnrichedSelectionName)
    )
    drawAnalysisHistConfiguratorZtoMuTauData.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm,
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure + '/' + dqmDirectoryAnalysis + meName_SVfitMass_norm
        ],
        name = ("%s_SVfitMass" % bgEnrichedSelectionName),
        title = ("%s: M(Muon + Tau), SVfit method" % bgEnrichedSelectionName)
    )

    drawAnalysisHistConfiguratorZtoMuTauMC.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm,
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure + '/' + dqmDirectoryAnalysis + meName_visMass_norm
        ],
        name = ("%s_visMass" % bgEnrichedSelectionName),
        title = ("%s: M_{vis}(Muon + Tau)" % bgEnrichedSelectionName)
    )
    drawAnalysisHistConfiguratorZtoMuTauMC.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm,
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure + '/' + dqmDirectoryAnalysis + meName_SVfitMass_norm
        ],
        name = ("%s_SVfitMass" % bgEnrichedSelectionName),
        title = ("%s: M(Muon + Tau), SVfit method" % bgEnrichedSelectionName)
    )

drawAnalysisHistConfiguratorZtoMuTauData.add(
    meNames = [
        '/template/harvested/' + dqmDirectoryData + '/' \
       + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_visMass_corrected_norm,
        '/analysis/harvested/' + dqmDirectoriesProcess['WplusJets'] + '/' \
       + dqmDirectoryAnalysis + meName_visMass_norm
    ],
    name = ("%s_visMass_corrected" % bgEnrichedSelectionName),
    title = ("%s: M_{vis}(Muon + Tau)" % bgEnrichedSelectionName)
)
drawAnalysisHistConfiguratorZtoMuTauData.add(
    meNames = [
        '/template/harvested/' + dqmDirectoryData + '/' \
       + dqmDirectoriesBgEnrichedSelections['WplusJetsEnriched'] + meName_SVfitMass_corrected_norm,
        '/analysis/harvested/' + dqmDirectoriesProcess['WplusJets'] + '/' \
       + dqmDirectoryAnalysis + meName_SVfitMass_norm
    ],
    name = ("%s_SVfitMass_corrected" % bgEnrichedSelectionName),
    title = ("%s: M(Muon + Tau), SVfit method" % bgEnrichedSelectionName)
)    

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
        analysis = copy.deepcopy(drawOption_red_eff)
    ),

    drawJobs = cms.PSet(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    indOutputFileName = cms.string('')
)

process.plotTemplateHistZtoMuTau = plotHistZtoMuTau.clone(
    drawJobs = drawTemplateHistConfiguratorZtoMuTau.configure(),
    indOutputFileName = cms.string('plotBgEstTemplateZtoMuTau_#PLOT#.png')
)

process.plotAnalysisHistZtoMuTauData = plotHistZtoMuTau.clone(
    drawJobs = drawAnalysisHistConfiguratorZtoMuTauData.configure(),
    indOutputFileName = cms.string('plotBgEstTemplateData_vs_AnalysisZtoMuTau_#PLOT#.png')
)

process.plotAnalysisHistZtoMuTauMC = plotHistZtoMuTau.clone(
    drawJobs = drawAnalysisHistConfiguratorZtoMuTauMC.configure(),
    indOutputFileName = cms.string('plotBgEstTemplateMC_vs_AnalysisZtoMuTau_#PLOT#.png')
)

process.saveBgEstTemplateHistZtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string(
        ##getHarvestingFilePath('ZtoMuTau_bgEstTemplate') + '/' + 'bgEstTemplateHistZtoMuTau_skimmed.root'
        '/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/2010Nov14/bgEstTemplateHistZtoMuTau_skimmed.root'
    ),
    outputCommands = cms.vstring(
        'drop *',
        'keep /template/harvested/ZtautauSum/*',
        'keep /template/harvested/Zmumu/*',
        'keep /template/harvested/qcdSum/*',
        'keep /template/harvested/WplusJets/*',
        'keep /template/harvested/TTplusJets/*',
        'keep /template/harvested/data/*',
        'keep /analysis/harvested/ZtautauSum/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVetoByMass/*',
        'keep /analysis/harvested/Zmumu/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVetoByMass/*',
        'keep /analysis/harvested/qcdSum/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVetoByMass/*',
        'keep /analysis/harvested/WplusJets/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVetoByMass/*',
        'keep /analysis/harvested/TTplusJets/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVetoByMass/*',
        'keep /analysis/harvested/smSum/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVetoByMass/*',
        'keep /analysis/harvested/data/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVetoByMass/*'
    )
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.p = cms.Path(
    #process.loadTemplateHistZtoMuTau_Ztautau
    process.loadTemplateHistZtoMuTau
   + process.loadAnalysisHistZtoMuTau
   + process.dumpDQMStore
   + process.rebinHistZtoMuTau
   + process.correctTemplateHistZtoMuTau
   + process.normalizeHistZtoMuTau
   ##+ process.dumpDQMStore 
   + process.plotTemplateHistZtoMuTau
   + process.plotAnalysisHistZtoMuTauData
   + process.plotAnalysisHistZtoMuTauMC
   + process.saveBgEstTemplateHistZtoMuTau 
)

# print-out all python configuration parameter information
#print process.dumpPython()


  
