import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# skim event sample for tau id. efficiency measurement
#--------------------------------------------------------------------------------

process = cms.Process("skimTauIdEffSample")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

# load definition of event selection 
process.load("TauAnalysis.TauIdEfficiency.filterTauIdEffSample_cfi")

# import definition of FEVT (RAW + RECO) event content
from Configuration.EventContent.EventContent_cff import *

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data2/veelken/CMSSW_4_1_x/skims/ZtoMuTau/DYtautau_spring11_powhegZ2_1_1_XvY.root'
        #'file:/data2/veelken/CMSSW_4_1_x/skims/ZtoMuTau/data2/veelken/CMSSW_4_1_x/skims/ZtoMuTau/data2011A_tauPlusX_AOD_1_1_MV9.root'
    )
)

isMC = True # use for MC
##isMC = False # use for Data
##HLTprocessName = "HLT" # use for 2011 Data
HLTprocessName = "REDIGI311X" # use for Spring'11 reprocessed MC
pfCandidateCollection = "particleFlow" # pile-up removal disabled
##pfCandidateCollection = "pfNoPileUp" # pile-up removal enabled
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__isMC = #isMC#
#__HLTprocessName = #HLTprocessName#
#__pfCandidateCollection = #pfCandidateCollection#
#__applyZrecoilCorrection = #applyZrecoilCorrection#
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define GlobalTag to be used for event reconstruction
if isMC:
    process.GlobalTag.globaltag = cms.string('START311_V2::All')
else:
    process.GlobalTag.globaltag = cms.string('GR_R_311_V2::All')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define skimming criteria
# (in order to be able to produce Tau Ntuple directly from unskimmed Monte Carlo/datasets;
#  HLT single jet trigger passed && either two CaloJets or two PFJets of Pt > 10 GeV within |eta| < 2.5)
process.load('TauAnalysis.TauIdEfficiency.filterTauIdEffSample_cfi')

process.hltMu.selector.src = cms.InputTag('TriggerResults::%s' % HLTprocessName)

if isMC:
    process.dataQualityFilters.remove(process.hltPhysicsDeclared)
    process.dataQualityFilters.remove(process.dcsstatus)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# produce collections of objects needed as input for PAT-tuple production
# (e.g. rerun reco::Tau identification algorithms with latest tags)
#
from TauAnalysis.TauIdEfficiency.tools.configurePrePatProduction import configurePrePatProduction

configurePrePatProduction(process, pfCandidateCollection = pfCandidateCollection, addGenInfo = isMC)

#process.prePatProductionSequence.remove(process.tautagging)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for configurating PAT-tuple production
from TauAnalysis.TauIdEfficiency.tools.configurePatTupleProductionTauIdEffMeasSpecific import *

patTupleConfig = configurePatTupleProductionTauIdEffMeasSpecific(
    process, hltProcess = HLTprocessName, addGenInfo = isMC, applyZrecoilCorrection = False, runSVfit = False)
#--------------------------------------------------------------------------------

process.p = cms.Path(
     process.prePatProductionSequence
   + process.patDefaultSequence
   + process.producePatTupleTauIdEffMeasSpecific
   #+ process.printEventContent
   #+ process.printGenParticleList
)    

process.load("Configuration.EventContent.EventContent_cff")
process.origFEVTSIMEventContent = copy.deepcopy(process.FEVTSIMEventContent)
process.origFEVTSIMEventContent.outputCommands.extend(
    cms.untracked.vstring('drop *_*_*_%s' % process.name_())
)    

process.skimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    process.origFEVTSIMEventContent,
    process.tauIdEffSampleEventSelection,
    fileName = cms.untracked.string("tauIdEffSample_RECO.root")
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# count events passing different skimming paths

process.DQMStore = cms.Service("DQMStore")

process.allEventCounts = cms.EDAnalyzer("DQMEventCounter",
    dqmDirectory = cms.string("EventCounts"),
    meName = cms.string("numEventsProcessed")
)
process.p += process.allEventCounts

process.passedMuonCaloTauSkimPath = process.allEventCounts.clone(
    meName = cms.string("numEventsPassedMuonCaloTauSkimPath")
)
process.muonCaloTauSkimPath += process.passedMuonCaloTauSkimPath

process.passedMuonPFTauFixedConeSkimPath = process.allEventCounts.clone(
    meName = cms.string("numEventsPassedMuonPFTauFixedConeSkimPath")
)
process.muonPFTauFixedConeSkimPath += process.passedMuonPFTauFixedConeSkimPath

process.passedMuonPFTauShrinkingConeSkimPath = process.allEventCounts.clone(
    meName = cms.string("numEventsPassedMuonPFTauShrinkingConeSkimPath")
)
process.muonPFTauShrinkingConeSkimPath += process.passedMuonPFTauShrinkingConeSkimPath

process.passedMuonPFTauHPSskimPath = process.allEventCounts.clone(
    meName = cms.string("numEventsPassedMuonPFTauHPSskimPath")
)
process.muonPFTauHPSskimPath += process.passedMuonPFTauHPSskimPath

process.passedMuonPFTauHPSpTaNCskimPath = process.allEventCounts.clone(
    meName = cms.string("numEventsPassedMuonPFTauHPSpTaNCskimPath")
)
process.muonPFTauHPSpTaNCskimPath += process.passedMuonPFTauHPSpTaNCskimPath

process.saveZtoMuTau_tauIdEffPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoMuTau_tauIdEff.root')
)

process.o = cms.EndPath(process.skimOutputModule + process.saveZtoMuTau_tauIdEffPlots)

# define order in which different paths are run
process.schedule = cms.Schedule(
    process.p,
    process.muonCaloTauSkimPath,
    process.muonPFTauFixedConeSkimPath,
    process.muonPFTauShrinkingConeSkimPath,
    process.muonPFTauHPSskimPath,
    process.muonPFTauHPSpTaNCskimPath,
    process.o
)

# print-out all python configuration parameter information
#print process.dumpPython()
