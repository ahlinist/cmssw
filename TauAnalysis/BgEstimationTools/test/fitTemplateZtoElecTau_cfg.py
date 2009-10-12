import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Apply "template" method for data-driven background estimation
# to Z --> e + tau-jet channel
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_cfi import *
from TauAnalysis.BgEstimationTools.bgEstNtupleDefinitionsZtoElecTau_cfi import *
from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *
from TauAnalysis.BgEstimationTools.templateHistDefinitions_cfi import *
from TauAnalysis.BgEstimationTools.tools.prodTemplateHistConfigurator import prodTemplateHistConfigurator
from TauAnalysis.BgEstimationTools.tools.drawTemplateHistConfigurator import drawTemplateHistConfigurator

processName = 'fitTemplateZtoElecTau'
process = cms.Process(processName)

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
# produce template histograms of visible electron + tau-jet mass distribution
#
# NOTE:
#  1.) template histogram for Ztautau signal process
#       taken from Z --> mu+ mu- events selected in (pseudo)data,
#       using MCEmbeddingTools
#  2.) distribution observed in (pseudo)data taken from plotsZtoElecTau_all.root file
#      produced by "official" analysis workflow documented at
#       https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideTauAnalysisZtoElecTau
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define event selection of background enriched samples
# from which template histograms are obtained
#--------------------------------------------------------------------------------

bgEstEventSelection_Zee = (
    "numDiTausZee >= 1 && electronTrackIsoZee_0 < 1. && electronEcalIsoZee_0 < 1."
#   " && tauDiscrAgainstElectronsZee_0 > 0.5 && numTightIdElectrons >= 2 && numJetsAlpha0point1Zee < 1"
    " && (tauDiscrAgainstElectronsZee_0 < 0.5 || numTightIdElectrons >= 2)"
#   " && diTauAbsChargeZee_0 < 0.5"
)

print("bgEstEventSelection_Zee = " + bgEstEventSelection_Zee)

bgEstEventSelection_WplusJets = (
#   "numDiTausWplusJets >= 1 && electronPtWplusJets_0 > 25. && electronTrackIsoWplusJets_0 < 1. && electronEcalIsoWplusJets_0 < 1."
#
# Note: electronPt cut improves WplusJets/QCD ratio by about a factor five,
#       but significantly shifts the electron + tau-jet visible invariant mass distribution towards higher values.
#       In order to supress QCD background contamination (on a statistical basis),
#       could extract W + jets template shape from difference in electron + tau-jet visible invariant mass distributions
#       of opposite sign - same sign electron and tau-jet combinations.
#      (SS/OS ratio is close to one for QCD background; significant charge asymmetry expected for W + jets background)
#    
    "numDiTausWplusJets >= 1 && electronTrackIsoWplusJets_0 < 1. && electronEcalIsoWplusJets_0 < 1."
#   " && tauTrackIsoDiscrWplusJets_0 < 0.5 && tauTrackIsoWplusJets_0 > 2. && tauDiscrAgainstElectronsWplusJets_0 > 0.5"
#
# Note: probability for quark/gluon jets to pass tau track and ECAL isolation criteria
#       is higher for low Pt than for high Pt jets; the consequence is that electron + tau-jet visible invariant mass distribution
#       gets shifted towards higher values in case tau track and ECAL isolation criteria are not applied.
#       For this reason, either need to apply tau track and ECAL isolation criteria in selection of W + jets background enriched sample
#       or correct for template shape distortion by reweighting
#      (would gain a factor of about 2.5 in event statistics; reweighting of tauPt distribution not implemented yet, however)
#    
    " && tauTrackIsoDiscrWplusJets_0 > 0.5 && tauEcalIsoDiscrWplusJets_0 > 0.5 && tauDiscrAgainstElectronsWplusJets_0 > 0.5"
    " && diTauMt1MEtWplusJets_0 > 30."
    " && numLooseIdElectrons < 2"
    " && numJetsAlpha0point1WplusJets < 1"
)

print("bgEstEventSelection_WplusJets = " + bgEstEventSelection_WplusJets)

bgEstEventSelection_TTplusJets = (
    "numDiTausTTplusJets >= 1 && electronTrackIsoTTplusJets_0 < 2. && electronEcalIsoTTplusJets_0 < 2."
    " && tauDiscrAgainstElectronsTTplusJets_0 > 0.5"
    " && diTauAbsChargeTTplusJets_0 < 0.5"
    " && numLooseIdElectrons < 2"
    " && ((numJetsEt40TTplusJets >= 1 && jetEt40bTaggingDiscrTrackCountingHighEffTTplusJets_0 > 4.5)"
    " || (numJetsEt40TTplusJets >= 2 && jetEt40bTaggingDiscrTrackCountingHighEffTTplusJets_1 > 4.5)"
    " || (numJetsEt40TTplusJets >= 3 && jetEt40bTaggingDiscrTrackCountingHighEffTTplusJets_2 > 4.5))"
    " && numJetsEt40TTplusJets >= 2 && numJetsEt60TTplusJets >= 1"
)

print("bgEstEventSelection_TTplusJets = " + bgEstEventSelection_TTplusJets)

bgEstEventSelection_gammaPlusJets = (
    ""
)

bgEstEventSelection_QCD = (
    "numDiTausQCD >= 1 && electronTrackIsoQCD_0 > 4. && electronEcalIsoQCD_0 > 4."
    " && tauDiscrAgainstElectronsQCD_0 > 0.5"
    " && numLooseIdElectrons < 2"
)

print("bgEstEventSelection_QCD = " + bgEstEventSelection_QCD)

#--------------------------------------------------------------------------------
# define directories in which histograms are stored in DQMStore
#--------------------------------------------------------------------------------

dqmDirectory_Ztautau_finalEvtSel_orig = 'harvested/Ztautau/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_Ztautau_finalEvtSel_norm = 'Ztautau/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_Ztautau_ZmumuTemplate = 'Ztautau/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
#dqmDirectory_Ztautau_ZmumuTemplate = 'Ztautau_from_selZmumu/pure/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
#dqmDirectory_Ztautau_ZmumuTemplate = 'Ztautau_from_selZmumu/pure/harvested/Ztautau_from_selZmumu/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_Ztautau_systematics = processName + '/Ztautau/systematics/'

dqmDirectory_Zee_finalEvtSel_orig = 'harvested/ZeePlusJets/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_Zee_finalEvtSel_norm = 'ZeePlusJets/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_Zee_bgEstEnriched_pure = processName + '/Zee/pure/'
dqmDirectory_Zee_bgEstEnriched_data = processName + '/Zee/data/'
dqmDirectory_Zee_systematics = processName + '/Zee/systematics/'

dqmDirectory_WplusJets_finalEvtSel_orig = 'harvested/WplusJets/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_WplusJets_finalEvtSel_norm = 'WplusJets/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_WplusJets_bgEstEnriched_pure = processName + '/WplusJets/pure/'
dqmDirectory_WplusJets_bgEstEnriched_data = processName + '/WplusJets/data/'
dqmDirectory_WplusJets_systematics = processName + '/WplusJets/systematics/'

dqmDirectory_TTplusJets_finalEvtSel_orig = 'harvested/TTplusJets/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_TTplusJets_finalEvtSel_norm = 'TTplusJets/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_TTplusJets_bgEstEnriched_pure = processName + '/TTplusJets/pure/'
dqmDirectory_TTplusJets_bgEstEnriched_data = processName + '/TTplusJets/data/'
dqmDirectory_TTplusJets_systematics = processName + '/TTplusJets/systematics/'

dqmDirectory_gammaPlusJets_finalEvtSel_orig = 'harvested/gammaPlusJetsSum/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_gammaPlusJets_finalEvtSel_norm = 'gammaPlusJetsSum/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_gammaPlusJets_bgEstEnriched_pure = processName + '/gammaPlusJetsSum/pure/'
dqmDirectory_gammaPlusJets_bgEstEnriched_data = processName + '/gammaPlusJetsSum/data/'
dqmDirectory_gammaPlusJets_systematics = processName + '/gammaPlusJetsSum/systematics/'

dqmDirectory_QCD_finalEvtSel_orig = 'harvested/qcdSum/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_QCD_finalEvtSel_norm = 'qcdSum/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_QCD_bgEstEnriched_pure = processName + '/QCD/pure/'
dqmDirectory_QCD_bgEstEnriched_data = processName + '/QCD/data/'
dqmDirectory_QCD_systematics = processName + '/QCD/systematics/'

dqmDirectory_data_finalEvtSel_orig = 'harvested/smSum/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'
dqmDirectory_data_finalEvtSel_extr = 'smSum/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/'

#--------------------------------------------------------------------------------
# define names of histograms used in fit
#--------------------------------------------------------------------------------

meName_diTauMvis12 = "VisMass"
meName_diTauMvis12_norm = "VisMassShape"

#--------------------------------------------------------------------------------
# define names of branches in Ntuple from which template histograms get produced
#--------------------------------------------------------------------------------

branchName_diTauMvis12_Zee = "diTauMvis12Zee_0"
branchName_diTauMvis12_WplusJets = "diTauMvis12WplusJets_0"
branchName_diTauMvis12_TTplusJets = "diTauMvis12TTplusJets_0"
branchName_diTauMvis12_gammaPlusJets = "diTauMvis12gammaPlusJets_0"
branchName_diTauMvis12_QCD = "diTauMvis12QCD_0"

#--------------------------------------------------------------------------------
# produce template histograms 
#--------------------------------------------------------------------------------

prodTemplateHistConfiguratorZeeEnriched = prodTemplateHistConfigurator(
    "prodTemplateHistBgEstZeeEnriched", prodTemplateHist, dqmDirectory = processName
)
prodTemplateHistConfiguratorZeeEnriched.addProcess("Ztautau", fileNames_Ztautau)
prodTemplateHistConfiguratorZeeEnriched.addProcess("Zee", fileNames_ZeePlusJets)
prodTemplateHistConfiguratorZeeEnriched.addProcess("WplusJets", fileNames_WplusJets)
prodTemplateHistConfiguratorZeeEnriched.addProcess("TTplusJets", fileNames_TTplusJets)
#prodTemplateHistConfiguratorZeeEnriched.addProcess("gammaPlusJets", fileNames_gammaPlusJetsSum)
prodTemplateHistConfiguratorZeeEnriched.addProcess("QCD", fileNames_qcdSum)
prodTemplateHistConfiguratorZeeEnriched.addProcess("data", fileNames_pseudoData)
prodTemplateHistConfiguratorZeeEnriched.addSelection("Zee", bgEstEventSelection_Zee)
prodTemplateHistConfiguratorZeeEnriched.addTemplate(meName_diTauMvis12_norm, branchName_diTauMvis12_Zee, 40, 0., 200.)

process.prodTemplateHistBgEstZeeEnriched = prodTemplateHistConfiguratorZeeEnriched.configure(process)

prodTemplateHistConfiguratorWplusJetsEnriched = prodTemplateHistConfigurator(
    "prodTemplateHistBgEstWplusJetsEnriched", prodTemplateHist, dqmDirectory = processName
)
prodTemplateHistConfiguratorWplusJetsEnriched.addProcess("Ztautau", fileNames_Ztautau)
prodTemplateHistConfiguratorWplusJetsEnriched.addProcess("Zee", fileNames_ZeePlusJets)
prodTemplateHistConfiguratorWplusJetsEnriched.addProcess("WplusJets", fileNames_WplusJets)
prodTemplateHistConfiguratorWplusJetsEnriched.addProcess("TTplusJets", fileNames_TTplusJets)
prodTemplateHistConfiguratorWplusJetsEnriched.addProcess("QCD", fileNames_qcdSum)
#prodTemplateHistConfiguratorWplusJetsEnriched.addProcess("gammaPlusJets", fileNames_gammaPlusJetsSum)
prodTemplateHistConfiguratorWplusJetsEnriched.addProcess("data", fileNames_pseudoData)
prodTemplateHistConfiguratorWplusJetsEnriched.addSelection("WplusJets", bgEstEventSelection_WplusJets,
                                                           kineEventReweight = "kineEventReweightWplusJets")
prodTemplateHistConfiguratorWplusJetsEnriched.addTemplate(meName_diTauMvis12_norm, branchName_diTauMvis12_WplusJets, 40, 0., 200.)

process.prodTemplateHistBgEstWplusJetsEnriched = prodTemplateHistConfiguratorWplusJetsEnriched.configure(process)

prodTemplateHistConfiguratorTTplusJetsEnriched = prodTemplateHistConfigurator(
    "prodTemplateHistBgEstTTplusJetsEnriched", prodTemplateHist, dqmDirectory = processName
)
prodTemplateHistConfiguratorTTplusJetsEnriched.addProcess("Ztautau", fileNames_Ztautau)
prodTemplateHistConfiguratorTTplusJetsEnriched.addProcess("Zee", fileNames_ZeePlusJets)
prodTemplateHistConfiguratorTTplusJetsEnriched.addProcess("WplusJets", fileNames_WplusJets)
prodTemplateHistConfiguratorTTplusJetsEnriched.addProcess("TTplusJets", fileNames_TTplusJets)
#prodTemplateHistConfiguratorTTplusJetsEnriched.addProcess("gammaPlusJets", fileNames_gammaPlusJetsSum)
prodTemplateHistConfiguratorTTplusJetsEnriched.addProcess("QCD", fileNames_qcdSum)
prodTemplateHistConfiguratorTTplusJetsEnriched.addProcess("data", fileNames_pseudoData)
prodTemplateHistConfiguratorTTplusJetsEnriched.addSelection("TTplusJets", bgEstEventSelection_TTplusJets)
prodTemplateHistConfiguratorTTplusJetsEnriched.addTemplate(meName_diTauMvis12_norm, branchName_diTauMvis12_TTplusJets, 40, 0., 200.)

process.prodTemplateHistBgEstTTplusJetsEnriched = prodTemplateHistConfiguratorTTplusJetsEnriched.configure(process)

prodTemplateHistConfiguratorGammaPlusJetsEnriched = prodTemplateHistConfigurator(
    "prodTemplateHistBgEstWplusJetsEnriched", prodTemplateHist, dqmDirectory = processName
)
prodTemplateHistConfiguratorGammaPlusJetsEnriched.addProcess("Ztautau", fileNames_Ztautau)
prodTemplateHistConfiguratorGammaPlusJetsEnriched.addProcess("Zee", fileNames_ZeePlusJets)
prodTemplateHistConfiguratorGammaPlusJetsEnriched.addProcess("WplusJets", fileNames_WplusJets)
prodTemplateHistConfiguratorGammaPlusJetsEnriched.addProcess("TTplusJets", fileNames_TTplusJets)
#prodTemplateHistConfiguratorGammaPlusJetsEnriched.addProcess("gammaPlusJets", fileNames_gammaPlusJetsSum)
prodTemplateHistConfiguratorGammaPlusJetsEnriched.addProcess("QCD", fileNames_qcdSum)
prodTemplateHistConfiguratorGammaPlusJetsEnriched.addProcess("data", fileNames_pseudoData)
prodTemplateHistConfiguratorGammaPlusJetsEnriched.addSelection("gammaPlusJets", bgEstEventSelection_gammaPlusJets)
prodTemplateHistConfiguratorGammaPlusJetsEnriched.addTemplate(meName_diTauMvis12_norm, branchName_diTauMvis12_gammaPlusJets, 40, 0., 200.)

process.prodTemplateHistBgEstGammaPlusJetsEnriched = prodTemplateHistConfiguratorGammaPlusJetsEnriched.configure(process)

prodTemplateHistConfiguratorQCDenriched = prodTemplateHistConfigurator(
    "prodTemplateHistBgEstWplusJetsEnriched", prodTemplateHist, dqmDirectory = processName
)
prodTemplateHistConfiguratorQCDenriched.addProcess("Ztautau", fileNames_Ztautau)
prodTemplateHistConfiguratorQCDenriched.addProcess("Zee", fileNames_ZeePlusJets)
prodTemplateHistConfiguratorQCDenriched.addProcess("WplusJets", fileNames_WplusJets)
prodTemplateHistConfiguratorQCDenriched.addProcess("TTplusJets", fileNames_TTplusJets)
#prodTemplateHistConfiguratorQCDenriched.addProcess("gammaPlusJets", fileNames_gammaPlusJetsSum)
prodTemplateHistConfiguratorQCDenriched.addProcess("QCD", fileNames_qcdSum)
prodTemplateHistConfiguratorQCDenriched.addProcess("data", fileNames_pseudoData)
prodTemplateHistConfiguratorQCDenriched.addSelection("QCD", bgEstEventSelection_QCD)
prodTemplateHistConfiguratorQCDenriched.addTemplate(meName_diTauMvis12_norm, branchName_diTauMvis12_QCD, 40, 0., 200.)

process.prodTemplateHistBgEstQCDenriched = prodTemplateHistConfiguratorQCDenriched.configure(process)

process.prodTemplateHistZtoElecTau = cms.Sequence( process.prodTemplateHistBgEstZeeEnriched 
                                                 * process.prodTemplateHistBgEstWplusJetsEnriched
                                                 * process.prodTemplateHistBgEstTTplusJetsEnriched
                                                 #* process.prodTemplateHistBgEstGammaPlusJetsEnriched
                                                 * process.prodTemplateHistBgEstQCDenriched )

#--------------------------------------------------------------------------------
# load template histogram of visible electron + tau-jet mass distribution
# produced from by MCEmbeddingTools from Z --> mu+ mu- events selected in (pseudo)data
#--------------------------------------------------------------------------------

process.loadTemplateHistZtoElecTau_Ztautau = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = cms.PSet(
        inputFileNames = cms.vstring('rfio:/castor/cern.ch/user/v/veelken/bgEstTemplates/ZtoElecTau_from_selZmumu.root'),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('Ztautau_from_selZmumu/pure/harvested')
    )
)

#--------------------------------------------------------------------------------
# normalize to unit area distribution of visible electron + tau-jet mass
# produced from by MCEmbeddingTools from Z --> mu+ mu- events selected in (pseudo)data
#--------------------------------------------------------------------------------

process.normalizeTemplateHistZtoElecTau_Ztautau = cms.EDAnalyzer("DQMHistNormalizer",
    config = cms.VPSet(
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_Ztautau_ZmumuTemplate + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_Ztautau_ZmumuTemplate + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm)
        )
    ),   
    norm = cms.double(1.)
)

#--------------------------------------------------------------------------------
# load distribution of visible electron + tau-jet mass
# expected for different signal/background processes and observed in (pseudo)data
# in events passing final analysis selection criteria
#--------------------------------------------------------------------------------

process.loadAnalysisHistZtoElecTau = cms.EDAnalyzer("DQMFileLoader",
    data = cms.PSet(
        inputFileNames = cms.vstring('rfio:/castor/cern.ch/user/v/veelken/bgEstPlots/ZtoElecTau/plotsZtoElecTau_all.root'),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('')
    )
)

process.extrAnalysisHistZtoElecTau = cms.EDAnalyzer("DQMHistScaler",
    dqmDirectory_input = cms.string(dqmDirectory_data_finalEvtSel_orig),
    dqmDirectory_output = cms.string(dqmDirectory_data_finalEvtSel_extr),
    scaleFactor = cms.double(1.)
)

#--------------------------------------------------------------------------------
# normalize to unit area distribution of visible electron + tau-jet mass
# in simulated signal/background events passing final analysis selection criteria
#--------------------------------------------------------------------------------

process.normalizeAnalysisHistZtoElecTau_Ztautau = cms.EDAnalyzer("DQMHistNormalizer",
    config = cms.VPSet(
        cms.PSet(
             meNameInput = cms.string(dqmDirectory_Ztautau_finalEvtSel_orig + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12),
             meNameOutput = cms.string(dqmDirectory_Ztautau_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm)
        )
    ),
    norm = cms.double(1.)
)

process.normalizeAnalysisHistZtoElecTau_Zee = cms.EDAnalyzer("DQMHistNormalizer",
    config = cms.VPSet(
        cms.PSet(
             meNameInput = cms.string(dqmDirectory_Zee_finalEvtSel_orig + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12),
             meNameOutput = cms.string(dqmDirectory_Zee_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm)
        )
    ),
    norm = cms.double(1.)
)

process.normalizeAnalysisHistZtoElecTau_WplusJets = cms.EDAnalyzer("DQMHistNormalizer",
    config = cms.VPSet(
        cms.PSet(
             meNameInput = cms.string(dqmDirectory_WplusJets_finalEvtSel_orig + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12),
             meNameOutput = cms.string(dqmDirectory_WplusJets_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm)
        )
    ),
    norm = cms.double(1.)
)

process.normalizeAnalysisHistZtoElecTau_TTplusJets = cms.EDAnalyzer("DQMHistNormalizer",
    config = cms.VPSet(
        cms.PSet(
             meNameInput = cms.string(dqmDirectory_TTplusJets_finalEvtSel_orig + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12),
             meNameOutput = cms.string(dqmDirectory_TTplusJets_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm)
        )
    ),
    norm = cms.double(1.)
)

process.normalizeAnalysisHistZtoElecTau_gammaPlusJets = cms.EDAnalyzer("DQMHistNormalizer",
    config = cms.VPSet(
        cms.PSet(
             meNameInput = cms.string(dqmDirectory_gammaPlusJets_finalEvtSel_orig + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12),
             meNameOutput = cms.string(dqmDirectory_gammaPlusJets_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm)
        )
    ),
    norm = cms.double(1.)
)

process.normalizeAnalysisHistZtoElecTau_QCD = cms.EDAnalyzer("DQMHistNormalizer",
    config = cms.VPSet(
        cms.PSet(
             meNameInput = cms.string(dqmDirectory_QCD_finalEvtSel_orig + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12),
             meNameOutput = cms.string(dqmDirectory_QCD_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm)
        )
    ),
    norm = cms.double(1.)
)

process.normalizeAnalysisHistZtoElecTau = cms.Sequence(
    process.normalizeAnalysisHistZtoElecTau_Ztautau
   + process.normalizeAnalysisHistZtoElecTau_Zee
   + process.normalizeAnalysisHistZtoElecTau_WplusJets
   + process.normalizeAnalysisHistZtoElecTau_TTplusJets
   #+ process.normalizeAnalysisHistZtoElecTau_gammaPlusJets
   + process.normalizeAnalysisHistZtoElecTau_QCD
)

#--------------------------------------------------------------------------------
# plot template histograms of "pure" Monte Carlo processes
# compared to the shapes determined by background enriched regions in (pseudo)Data
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoElecTau = drawTemplateHistConfigurator(
    template = drawJobTemplateHist
)

#--------------------------------------------------------------------------------
# define draw jobs for Z --> tau+ tau- signal
#
# NOTE: backgrounds contributing to Z --> mu+ mu- sample from which
#       template histogram for Ztautau signal process is determined using MCEmbeddingTools
#       not included yet, so use "pure" sample as approximation for sample determined in "deta"
#       for the time being...
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoElecTau.add(
    meNames = [
        dqmDirectory_Ztautau_ZmumuTemplate + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm,
        dqmDirectory_Ztautau_ZmumuTemplate + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm,
        dqmDirectory_Ztautau_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm
    ],
    name = "Ztautau_diTauMvis12",
    title = "M_{vis}^{e + #tau-jet} in Z #rightarrow #tau^{+} #tau^{-} Signal" 
)

#--------------------------------------------------------------------------------
# define draw jobs for Z --> mu+ mu- background
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoElecTau.add(
    meNames = [
        dqmDirectory_Zee_bgEstEnriched_data + meName_diTauMvis12_norm,
        dqmDirectory_Zee_bgEstEnriched_pure + meName_diTauMvis12_norm,
        dqmDirectory_Zee_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm
    ],
    name = "Zee_diTauMvis12",
    title = "M_{vis}^{e + #tau-jet} in Z #rightarrow e^{+} e^{-} Background"
)

#--------------------------------------------------------------------------------
# define draw jobs for W + jets background
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoElecTau.add(
    meNames = [
        dqmDirectory_WplusJets_bgEstEnriched_data + meName_diTauMvis12_norm,
        dqmDirectory_WplusJets_bgEstEnriched_pure + meName_diTauMvis12_norm,
        dqmDirectory_WplusJets_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm
    ],
    name = "WplusJets_diTauMvis12",
    title = "M_{vis}^{e + #tau-jet} in W + jets Background"
)

#--------------------------------------------------------------------------------
# define draw jobs for TTbar + jets background
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoElecTau.add(
    meNames = [
        dqmDirectory_TTplusJets_bgEstEnriched_data + meName_diTauMvis12_norm,
        dqmDirectory_TTplusJets_bgEstEnriched_pure + meName_diTauMvis12_norm,
        dqmDirectory_TTplusJets_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm
    ],
    name = "TTplusJets_diTauMvis12",
    title = "M_{vis}^{e + #tau-jet} in t#bar{t} + jets Background"
)

#--------------------------------------------------------------------------------
# define draw jobs for gamma + jets background
#--------------------------------------------------------------------------------

##drawTemplateHistConfiguratorZtoElecTau.add(
##    meNames = [
##        dqmDirectory_gammaPlusJets_bgEstEnriched_data + meName_diTauMvis12_norm,
##        dqmDirectory_gammaPlusJets_bgEstEnriched_pure + meName_diTauMvis12_norm,
##        dqmDirectory_gammaPlusJets_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm
##    ],
##    name = "gammaPlusJets_diTauMvis12",
##    title = "M_{vis}^{e + #tau-jet} in #gamma + jets Background"
##)

#--------------------------------------------------------------------------------
# define draw jobs for QCD background
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoElecTau.add(
    meNames = [
        dqmDirectory_QCD_bgEstEnriched_data + meName_diTauMvis12_norm,
        dqmDirectory_QCD_bgEstEnriched_pure + meName_diTauMvis12_norm,
        dqmDirectory_QCD_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm
    ],
    name = "QCD_diTauMvis12",
    title = "M_{vis}^{e + #tau-jet} in QCD Background"
)

process.plotTemplateHistZtoElecTau = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
        bgEstData = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = drawJobTemplateHist.plots[0].legendEntry,
            type = cms.string('smMC')
        ),
        bgEstPure = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = drawJobTemplateHist.plots[1].legendEntry,
            type = cms.string('smMC')
        ),
        finalEvtSel = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = drawJobTemplateHist.plots[2].legendEntry,
            type = cms.string('smMC')
        )
    ),

    xAxes = cms.PSet(
        M = copy.deepcopy(xAxis_mass)
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
        bgEstData = copy.deepcopy(drawOption_darkBlue_eff),
        bgEstPure = copy.deepcopy(drawOption_lightBlue_eff),
        finalEvtSel = copy.deepcopy(drawOption_red_eff)
    ),

    drawJobs = drawTemplateHistConfiguratorZtoElecTau.configure(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsTemplateHistZtoElecTau.ps')
    indOutputFileName = cms.string('plotTemplateHistZtoElecTau_#PLOT#.eps')
)

#--------------------------------------------------------------------------------
# produce auxiliary histograms representing bias of visible electron + tau-jet mass distribution
# introduced by differences in event selection between final analysis and background enriched samples
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

process.prodSysBiasHistZtoElecTau_Ztautau = cms.EDAnalyzer("DQMHistSubtractor",
    config = cms.VPSet(
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_Ztautau_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_Ztautau_ZmumuTemplate + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_Ztautau_systematics + "bias" + "/" + meName_diTauMvis12_norm)
        )
    )    
)

process.prodSysBiasHistZtoElecTau_Zee = cms.EDAnalyzer("DQMHistSubtractor",
    config = cms.VPSet(
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_Zee_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_Zee_bgEstEnriched_data + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_Zee_systematics + "bias" + "/" + meName_diTauMvis12_norm)
        )
    )                                                       
)

process.prodSysBiasHistZtoElecTau_WplusJets = cms.EDAnalyzer("DQMHistSubtractor",
    config = cms.VPSet(
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_WplusJets_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_WplusJets_bgEstEnriched_data + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_WplusJets_systematics + "bias" + "/" + meName_diTauMvis12_norm)
        )
    )                                                       
)

process.prodSysBiasHistZtoElecTau_TTplusJets = cms.EDAnalyzer("DQMHistSubtractor",
    config = cms.VPSet(
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_TTplusJets_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_TTplusJets_bgEstEnriched_data + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_TTplusJets_systematics + "bias" + "/" + meName_diTauMvis12_norm)
        )
    )                                                       
)

process.prodSysBiasHistZtoElecTau_gammaPlusJets = cms.EDAnalyzer("DQMHistSubtractor",
    config = cms.VPSet(
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_gammaPlusJets_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_gammaPlusJets_bgEstEnriched_data + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_gammaPlusJets_systematics + "bias" + "/" + meName_diTauMvis12_norm)
        )
    )                                                       
)

process.prodSysBiasHistZtoElecTau_QCD = cms.EDAnalyzer("DQMHistSubtractor",
    config = cms.VPSet(
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_QCD_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_QCD_bgEstEnriched_data + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_QCD_systematics + "bias" + "/" + meName_diTauMvis12_norm)
        )
    )                                                       
)

process.prodSysBiasHistZtoElecTau = cms.Sequence(
    process.prodSysBiasHistZtoElecTau_Ztautau
   + process.prodSysBiasHistZtoElecTau_Zee
   + process.prodSysBiasHistZtoElecTau_WplusJets
   + process.prodSysBiasHistZtoElecTau_TTplusJets
   #+ process.prodSysBiasHistZtoElecTau_gammaPlusJets
   + process.prodSysBiasHistZtoElecTau_QCD
)

#--------------------------------------------------------------------------------
# store all histograms into ROOT file
#--------------------------------------------------------------------------------

process.saveAllHistZtoElecTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('bgEstTemplatesZtoElecTau.root'),
    drop = cms.vstring('harvested/')                       
)

#--------------------------------------------------------------------------------
# fit template histograms to distribution of visible electron + tau-jet mass in (pseudo)Data,
# in order to determine normalization factors of individual background processes
#--------------------------------------------------------------------------------

process.fitZtoElecTau = cms.EDAnalyzer("TemplateBgEstFit",                                          
    processes = cms.PSet(
        Ztautau = cms.PSet(
            templates = cms.PSet(
                diTauMvis12 = cms.PSet(
                    meName = cms.string(dqmDirectory_Ztautau_ZmumuTemplate + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm),
                    #meName = cms.string(dqmDirectory_Ztautau_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm)
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(250.)
            ),
            drawOptions = drawOption_Ztautau                
        ),
        Zee = cms.PSet(
            templates = cms.PSet(
                diTauMvis12 = cms.PSet(                    
                    #meName = cms.string(dqmDirectory_Zee_bgEstEnriched_data + meName_diTauMvis12_norm)
                    meName = cms.string(dqmDirectory_Zee_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm)
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(25.)
            ),
            drawOptions = drawOption_Zee
        ),
        WplusJets = cms.PSet(
            templates = cms.PSet(
                diTauMvis12 = cms.PSet(
                    meName = cms.string(dqmDirectory_WplusJets_bgEstEnriched_data + meName_diTauMvis12_norm),
                    #meName = cms.string(dqmDirectory_WplusJets_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm)
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(100.)
            ),
            drawOptions = drawOption_WplusJets
        ),
        TTplusJets = cms.PSet(
            templates = cms.PSet(
                diTauMvis12 = cms.PSet(
                    meName = cms.string(dqmDirectory_TTplusJets_bgEstEnriched_data + meName_diTauMvis12_norm)
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(25.)
            ),
            drawOptions = drawOption_TTplusJets
        ),
        ##gammaPlusJets = cms.PSet(
        ##    templates = cms.PSet(
        ##        diTauMvis12 = cms.PSet(
        ##            meName = cms.string(dqmDirectory_gammaPlusJets_bgEstEnriched_data + meName_diTauMvis12_norm)
        ##        )
        ##    ),    
        ##    norm = cms.PSet(
        ##        initial = cms.double(25.)
        ##    ),
        ##    drawOptions = drawOption_gammaPlusJets
        ##),
        QCD = cms.PSet(
            templates = cms.PSet(
                diTauMvis12 = cms.PSet(
                    meName = cms.string(dqmDirectory_QCD_bgEstEnriched_data + meName_diTauMvis12_norm),
                    #meName = cms.string(dqmDirectory_QCD_finalEvtSel_norm + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12_norm)
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(100.)
            ),
            drawOptions = drawOption_QCD,
        )
    ),

    # use "pseudo" data-samples consisting of all Monte Carlo processes for testing                      
    data = cms.PSet(
        distributions = cms.PSet(
            diTauMvis12 = cms.PSet(
                meName = cms.string(dqmDirectory_data_finalEvtSel_extr + "DiTauCandidateQuantities" + "/" + meName_diTauMvis12)
            )
        )
    ),

    fit = cms.PSet(
        mode = cms.string("Nd"),
        variables = cms.PSet(
            diTauMvis12 = cms.PSet(
               name = cms.string("diTauMvis12"),
               title = cms.string("M_{vis}^{e + #tau-jet}"),
               xMin = cms.double(30.),
               xMax = cms.double(80.)
            )
        ),
        cutUnfittedRegion = cms.bool(False),
        #cutUnfittedRegion = cms.bool(True),
        verbosity = cms.PSet(
            printLevel = cms.int32(1),
            printWarnings = cms.bool(True)
        )
    ),

    estStatUncertainties = cms.PSet(
        numSamplings = cms.int32(100),
        chi2redMax = cms.double(3.),
        verbosity = cms.PSet(
            printLevel = cms.int32(-1),
            printWarnings = cms.bool(False)
        )
    ),

    estSysUncertainties = cms.PSet(
        fluctuations = cms.PSet(
            bias = cms.PSet(
                meNames = cms.PSet(
                    Ztautau = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_Ztautau_systematics + "bias" + "/" + meName_diTauMvis12_norm)
                    ),
                    Zee = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_Zee_systematics + "bias" + "/" + meName_diTauMvis12_norm)
                    ),
                    WplusJets = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_WplusJets_systematics + "bias" + "/" + meName_diTauMvis12_norm)
                    ),
                    TTplusJets = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_TTplusJets_systematics + "bias" + "/" + meName_diTauMvis12_norm)
                    ),
                    ##gammaPlusJets = cms.PSet(
                    ##    diTauMvis12 = cms.string(dqmDirectory_gammaPlusJets_systematics + "bias" + "/" + meName_diTauMvis12_norm)
                    ##),
                    QCD = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_QCD_systematics + "bias" + "/" + meName_diTauMvis12_norm)
                    )
                ),
                pullRMS = cms.double(1.),
                pullMin = cms.double(0.),
                pullMax = cms.double(1.),
                mode = cms.string("coherent") # coherent/incoherent
            )
        ),       
        numSamplings = cms.int32(100),
        chi2redMax = cms.double(3.),
        verbosity = cms.PSet(
            printLevel = cms.int32(-1),
            printWarnings = cms.bool(False)
        )
    ),                                     

    output = cms.PSet(
        controlPlots = cms.PSet(
            fileName = cms.string("./plots/fitTemplateZtoElecTau_#PLOT#.eps")
        )
    )                                      
)                          

process.prodAllHistZtoElecTau = cms.Sequence(
    process.prodTemplateHistZtoElecTau
   #+ process.loadTemplateHistZtoElecTau_Ztautau
   #+ process.normalizeTemplateHistZtoElecTau_Ztautau
   + process.loadAnalysisHistZtoElecTau
   + process.extrAnalysisHistZtoElecTau
   + process.normalizeAnalysisHistZtoElecTau
   + process.prodSysBiasHistZtoElecTau
   + process.saveAllHistZtoElecTau  
)

process.loadAllHistZtoElecTau = cms.EDAnalyzer("DQMFileLoader",
    all = cms.PSet(
        inputFileNames = cms.vstring('bgEstTemplatesZtoElecTau.root'),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('')
    )
)

process.p = cms.Path(
   #process.prodAllHistZtoElecTau
    process.loadAllHistZtoElecTau
   + process.plotTemplateHistZtoElecTau
   + process.fitZtoElecTau
)

# print-out all python configuration parameter information
#print process.dumpPython()


  
