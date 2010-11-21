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
# load template histogram for Z --> tau+ tau- signal events,
# produced from by MCEmbeddingTools from Z --> mu+ mu- events selected in (pseudo)data
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
            getHarvestingFilePath('ZtoMuTau_bgEstTemplate') + '/' + 'plotsZtoMuTau_bgEstTemplate_all.root'
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
            getHarvestingFilePath('ZtoMuTau') + '/' + 'plotsZtoMuTau_all.root'
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('/analysis')
    )
)

#--------------------------------------------------------------------------------
# normalize to unit area distribution of visible muon + tau-jet mass for Z --> tau+ tau- signal
# and different types of background events
#--------------------------------------------------------------------------------

jobsHistNormalization = []

for processName, dqmDirectoryProcess in dqmDirectoriesProcess.items():
    for bgEnrichedSelectionName, dqmDirectoryBgEnrichedSelection in dqmDirectoriesBgEnrichedSelections.items():
        jobsHistNormalization.append(cms.PSet(
            meName_input = cms.string(
                '/template/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass
            ),
            meName_output = cms.string(
                '/template/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm
            )
        ))
        jobsHistNormalization.append(cms.PSet(
            meName_input = cms.string(
                '/template/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass
            ),
            meName_output = cms.string(
                '/template/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
            )
        ))

    jobsHistNormalization.append(cms.PSet(
        meName_input = cms.string(
            '/analysis/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryAnalysis + meName_visMass
        ),
        meName_output = cms.string(
            '/analysis/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryAnalysis + meName_visMass_norm
        )
    ))
    jobsHistNormalization.append(cms.PSet(
        meName_input = cms.string(
            '/analysis/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryAnalysis + meName_SVfitMass
        ),
        meName_output = cms.string(
            '/analysis/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryAnalysis + meName_SVfitMass_norm
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
        name = ("bgEstTemplate_%s_visMass" % bgEnrichedSelectionName),
        title = "M_{vis}(Muon + Tau)"
    )
    drawTemplateHistConfiguratorZtoMuTau.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm,
            '/template/harvested/' + dqmDirectoryBgEstMC_pure + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm,
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm
        ],
        name = ("bgEstTemplate_%s_SVfitMass" % bgEnrichedSelectionName),
        title = "M(Muon + Tau), SVfit method"
    )

    drawAnalysisHistConfiguratorZtoMuTauData.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm,
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure + '/' + dqmDirectoryAnalysis + meName_visMass_norm
        ],
        name = ("analysis_vs_bgEstTemplateData_%s_visMass" % bgEnrichedSelectionName),
        title = "M_{vis}(Muon + Tau)"
    )
    drawAnalysisHistConfiguratorZtoMuTauData.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryData + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm,
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure + '/' + dqmDirectoryAnalysis + meName_SVfitMass_norm
        ],
        name = ("analysis_vs_bgEstTemplateData_%s_SVfitMass" % bgEnrichedSelectionName),
        title = "M(Muon + Tau), SVfit method"
    )

    drawAnalysisHistConfiguratorZtoMuTauMC.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum + '/' + dqmDirectoryBgEnrichedSelection + meName_visMass_norm,
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure + '/' + dqmDirectoryAnalysis + meName_visMass_norm
        ],
        name = ("analysis_vs_bgEstTemplateMC_%s_visMass" % bgEnrichedSelectionName),
        title = "M_{vis}(Muon + Tau)"
    )
    drawAnalysisHistConfiguratorZtoMuTauMC.add(
        meNames = [
            '/template/harvested/' + dqmDirectoryBgEstMC_smSum + '/' + dqmDirectoryBgEnrichedSelection + meName_SVfitMass_norm,
            '/analysis/harvested/' + dqmDirectoryAnalysisMC_pure + '/' + dqmDirectoryAnalysis + meName_SVfitMass_norm
        ],
        name = ("analysis_vs_bgEstTemplateMC_%s_SVfitMass" % bgEnrichedSelectionName),
        title = "M(Muon + Tau), SVfit method"
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

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.p = cms.Path(
    #process.loadTemplateHistZtoMuTau_Ztautau
    process.loadTemplateHistZtoMuTau
   + process.loadAnalysisHistZtoMuTau
   + process.dumpDQMStore
   + process.normalizeHistZtoMuTau
   ##+ process.dumpDQMStore
   + process.plotTemplateHistZtoMuTau
   + process.plotAnalysisHistZtoMuTauData
   + process.plotAnalysisHistZtoMuTauMC
)

# print-out all python configuration parameter information
#print process.dumpPython()


  
