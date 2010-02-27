import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Make control plots comparing background contribution estimated by fake-rate technique
# with background contribution estimated by template method
# in the Z --> mu + tau-jet channel
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

# uncomment next line to make plots for 10 TeV centre-of-mass energy
#from TauAnalysis.Configuration.plotZtoMuTau_processes_10TeV_cfi import *
# uncomment next line to make plots for 7 TeV centre-of-mass energy
from TauAnalysis.Configuration.plotZtoMuTau_processes_7TeV_cfi import *
from TauAnalysis.Configuration.plotZtoMuTau_drawJobs_cfi import *
from TauAnalysis.Configuration.plotZtoMuTau_cff import loadZtoMuTau
from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

from TauAnalysis.BgEstimationTools.tools.drawFakeRateHistConfigurator import drawFakeRateHistConfigurator
from TauAnalysis.BgEstimationTools.tools.fakeRateTools import reconfigDQMFileLoader

process = cms.Process('makeBgEstMethodCompZtoMuTauPlots')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
process.loadZtoMuTau = cms.EDAnalyzer("DQMFileLoader",
    tauIdDiscr = cms.PSet(
        inputFileNames = cms.vstring(
            'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/plots/ZtoMuTau/7TeV/plotsZtoMuTau_all.root'
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('methodTauIdDiscr')
    )
)

process.loadBgEstFakeRateZtoMuTau = cms.EDAnalyzer("DQMFileLoader",
    tauIdDiscr = cms.PSet(
        inputFileNames = cms.vstring(
            'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/bgEstPlots/ZtoMuTau_frSimple/10TeV/plotsZtoMuTau_bgEstFakeRate.root'
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('methodTauFakeRate')
    )
)

process.loadBgEstTemplateFitZtoMuTau = cms.EDAnalyzer("DQMFileLoader",
    results = cms.PSet(
        inputFileNames = cms.vstring(
            'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/bgEstPlots/ZtoMuTau_bgEstTemplates/7TeV/fitBgEstTemplateZtoMuTau_results.root'
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('methodBgEstTemplateFit')
    )
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
process.addBgEstFakeRateZtoMuTau_smBgSum = cms.EDAnalyzer("DQMHistAdder",
    smBgSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'methodTauFakeRate/tauFakeRate/harvested/Zmumu',
            'methodTauFakeRate/tauFakeRate/harvested/WplusJets',
            'methodTauFakeRate/tauFakeRate/harvested/TTplusJets',
            'methodTauFakeRate/tauFakeRate/harvested/QCD'
        ),
        dqmDirectory_output = cms.string('methodTauFakeRate/tauFakeRate/harvested/smBgSum')
    )
)

process.compBgEstFakeRateZtoMuTau_smBgSum = cms.Sequence(process.addBgEstFakeRateZtoMuTau_smBgSum)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
##process.scaleBgEstTemplateFitZtoMuTau_Ztautau = cms.EDAnalyzer("DQMHistScaler",
##    dqmDirectory_input = cms.string('methodTauIdDiscr/harvested/Ztautau/afterEvtSelDiMuPairZmumuHypothesisVeto'),
##    dqmDirectory_output = cms.string('methodBgEstTemplateFit/template/Ztautau'),
##    meNameScaleFactor = cms.string('methodBgEstTemplateFit/fitBgEstTemplateZtoMuTau/fitResults/Ztautau/norm/value'),
##    meTypeScaleFactor = cms.string('real')                                                            
##)

dqmDirectory_template = 'methodBgEstTemplateFit/template/harvested/smSum/BgEstTemplateAnalyzer_'

process.scaleBgEstTemplateFitZtoMuTau_ZmumuJetMisId = cms.EDAnalyzer("DQMHistScaler",
    dqmDirectory_input = cms.string(dqmDirectory_template + 'ZmumuJetMisIdEnriched/afterDiMuonPairInvMassBgEstZmumuJetMisIdEnriched'),
    dqmDirectory_output = cms.string('methodBgEstTemplateFit/template/ZmumuJetMisId'),
    meNameScaleFactor = cms.string('methodBgEstTemplateFit/fitBgEstTemplateZtoMuTau/fitResults/ZmumuJetMisId/norm/value#a1#s1'),
    meTypeScaleFactor = cms.string('real')                                                            
)

process.scaleBgEstTemplateFitZtoMuTau_ZmumuMuonMisId = cms.EDAnalyzer("DQMHistScaler",
    dqmDirectory_input = cms.string(dqmDirectory_template + 'ZmumuMuonMisIdEnriched/afterDiMuonPairBgEstZmumuMuonMisIdEnriched'),
    dqmDirectory_output = cms.string('methodBgEstTemplateFit/template/ZmumuMuonMisId'),
    meNameScaleFactor = cms.string('methodBgEstTemplateFit/fitBgEstTemplateZtoMuTau/fitResults/ZmumuMuonMisId/norm/value#a1#s1'),
    meTypeScaleFactor = cms.string('real')                                                            
)

process.scaleBgEstTemplateFitZtoMuTau_WplusJets = cms.EDAnalyzer("DQMHistScaler",
    dqmDirectory_input = cms.string(dqmDirectory_template + 'WplusJetsEnriched/afterDiMuonVetoBgEstWplusJetsEnriched'),
    dqmDirectory_output = cms.string('methodBgEstTemplateFit/template/WplusJets'),
    meNameScaleFactor = cms.string('methodBgEstTemplateFit/fitBgEstTemplateZtoMuTau/fitResults/WplusJets/norm/value#a1#s1'),
    meTypeScaleFactor = cms.string('real')                                                            
)

process.scaleBgEstTemplateFitZtoMuTau_TTplusJets = cms.EDAnalyzer("DQMHistScaler",
    dqmDirectory_input = cms.string(dqmDirectory_template + 'TTplusJetsEnriched/afterDiMuonVetoBgEstTTplusJetsEnriched'),
    dqmDirectory_output = cms.string('methodBgEstTemplateFit/template/TTplusJets'),
    meNameScaleFactor = cms.string('methodBgEstTemplateFit/fitBgEstTemplateZtoMuTau/fitResults/TTplusJets/norm/value#a1#s1'),
    meTypeScaleFactor = cms.string('real')                                                            
)

process.scaleBgEstTemplateFitZtoMuTau_QCD = cms.EDAnalyzer("DQMHistScaler",
    dqmDirectory_input = cms.string(dqmDirectory_template + 'QCDenriched/afterDiMuonVetoBgEstQCDenriched'),
    dqmDirectory_output = cms.string('methodBgEstTemplateFit/template/QCD'),
    meNameScaleFactor = cms.string('methodBgEstTemplateFit/fitBgEstTemplateZtoMuTau/fitResults/QCD/norm/value#a1#s1'),
    meTypeScaleFactor = cms.string('real')                                                            
)

process.addBgEstTemplateFitZtoMuTau_smBgSum = cms.EDAnalyzer("DQMHistAdder",
    smBgSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'methodBgEstTemplateFit/template/ZmumuJetMisId',
            'methodBgEstTemplateFit/template/ZmumuMuonMisId',
            'methodBgEstTemplateFit/template/WplusJets',
            'methodBgEstTemplateFit/template/TTplusJets',
            'methodBgEstTemplateFit/template/QCD'
        ),
        dqmDirectory_output = cms.string('methodBgEstTemplateFit/template/smBgSum')
    )
)

process.compBgEstTemplateFitZtoMuTau_smBgSum = cms.Sequence(
    process.scaleBgEstTemplateFitZtoMuTau_ZmumuJetMisId + process.scaleBgEstTemplateFitZtoMuTau_ZmumuMuonMisId
   + process.scaleBgEstTemplateFitZtoMuTau_WplusJets
   + process.scaleBgEstTemplateFitZtoMuTau_TTplusJets
   + process.scaleBgEstTemplateFitZtoMuTau_QCD
   + process.addBgEstTemplateFitZtoMuTau_smBgSum
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
process.plotBgEstMethodCompZtoMuTau = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
        tauIdDiscr = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('tauIdDiscr'),
            type = cms.string('smMC')
        ),
        tauFakeRate = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('tauFakeRate'),
            type = cms.string('smMC')
        ),
        bgEstTemplateFit = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('bgEstTemplateFit'),
            type = cms.string('smMC')
        )
    ),

    xAxes = cms.PSet(
        Pt = copy.deepcopy(xAxis_pt),
        Eta = copy.deepcopy(xAxis_eta),
        Phi = copy.deepcopy(xAxis_phi),
        IPxy = copy.deepcopy(xAxis_ipXY),
        IPz = copy.deepcopy(xAxis_ipZ),
        dR = copy.deepcopy(xAxis_dR),
        dPhi = copy.deepcopy(xAxis_dPhi),
        prob = copy.deepcopy(xAxis_prob),
        posZ = copy.deepcopy(xAxis_posZ),
        Mt = copy.deepcopy(xAxis_transMass),
        Mass = copy.deepcopy(xAxis_mass),
        N = copy.deepcopy(xAxis_num),
        PdgId = copy.deepcopy(xAxis_pdgId),
        GeV = copy.deepcopy(xAxis_GeV),
        unlabeled = copy.deepcopy(xAxis_unlabeled)
    ),

    yAxes = cms.PSet(                         
        numEntries_linear = copy.deepcopy(yAxis_numEntries_linear),
        numEntries_log = copy.deepcopy(yAxis_numEntries_log)
    ),

    legends = cms.PSet(
        regular = copy.deepcopy(legend_regular)
    ),

    labels = cms.PSet(
        mcNormScale = copy.deepcopy(label_mcNormScale)
    ),

    drawOptionEntries = cms.PSet(
        tauIdDiscr = copy.deepcopy(drawOption_black_separate),
        tauFakeRate = copy.deepcopy(drawOption_lightBlue_separate),
        bgEstTemplateFit = copy.deepcopy(drawOption_red_separate)
    ),                                                 
                              
    drawJobs = cms.PSet(
        mVisible = cms.PSet(
            plots = cms.VPSet(
                cms.PSet(
                    process = cms.string('tauIdDiscr'),
                    dqmMonitorElements = cms.vstring('methodTauIdDiscr/harvested/smBgSum/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVeto/DiTauCandidateQuantities/VisMass'),
                    drawOptionEntry = cms.string('tauIdDiscr'),
                    legendEntry = cms.string('Final Analysis')
                ), 
                cms.PSet(
                    process = cms.string('tauIdFakeRate'),
                    dqmMonitorElements = cms.vstring('methodTauFakeRate/tauFakeRate/harvested/smBgSum/frSysUncertainty/afterEvtSelDiMuPairZmumuHypothesisVeto/DiTauCandidateQuantities/VisMass'),
                    drawOptionEntry = cms.string('tauFakeRate'),
                    legendEntry = cms.string('Fake-Rate technique')
                ),
                cms.PSet(
                    process = cms.string('bgEstTemplateFit'),
                    dqmMonitorElements = cms.vstring('methodBgEstTemplateFit/template/smBgSum/DiTauCandidateQuantities/VisMass'),
                    drawOptionEntry = cms.string('bgEstTemplateFit'),
                    legendEntry = cms.string('Template method')
                )
            ),
            title = cms.string('M_{vis}(Muon + Tau)'),
            xAxis = cms.string('unlabeled'),
            yAxis = cms.string('numEntries_log'),
            legend = cms.string('regular'),
            labels = cms.vstring('mcNormScale')
        )
    ),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsBgEstMethodCompZtoMuTau.ps')
    indOutputFileName = cms.string('plotBgEstMethodCompZtoMuTau_#PLOT#.png')
)
#--------------------------------------------------------------------------------

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.p = cms.Path(
    process.loadZtoMuTau
   + process.loadBgEstFakeRateZtoMuTau
   + process.loadBgEstTemplateFitZtoMuTau
   + process.dumpDQMStore
   + process.compBgEstFakeRateZtoMuTau_smBgSum
   + process.compBgEstTemplateFitZtoMuTau_smBgSum
   + process.plotBgEstMethodCompZtoMuTau
)

# print-out all python configuration parameter information
print process.dumpPython()
