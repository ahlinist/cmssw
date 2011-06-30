import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# skim event sample for tau id. efficiency measurement
#--------------------------------------------------------------------------------

process = cms.Process("skimTauIdEffSample")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 10000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

# load definition of event selection 
process.load("TauAnalysis.TauIdEfficiency.filterTauIdEffSample_cfi")

# import definition of FEVT (RAW + RECO) event content
from Configuration.EventContent.EventContent_cff import *

#--------------------------------------------------------------------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data2/friis/CMSSW_4_2_X/skims/06-27-MatthewsZTTEvents/crab_0_110627_082505/ZTTCands_merged_v1.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

##isMC = True # use for MC
isMC = False # use for Data
#HLTprocessName = "HLT" # use for 2011 Data
HLTprocessName = "HLT" # use for Summer'11 MC
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
    process.GlobalTag.globaltag = cms.string('START42_V12::All')
else:
    process.GlobalTag.globaltag = cms.string('GR_R_42_V14::All')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define skimming criteria
# (in order to be able to produce Tau Ntuple directly from unskimmed Monte Carlo/datasets;
#  HLT single jet trigger passed && either two CaloJets or two PFJets of Pt > 10 GeV within |eta| < 2.5)
process.load('TauAnalysis.TauIdEfficiency.filterTauIdEffSample_cfi')

process.hltMu.selector.src = cms.InputTag('TriggerResults::%s' % HLTprocessName)
process.hltMu.selector.triggerPaths = cms.vstring(
    'HLT_IsoMu12_v1',
    'HLT_Mu15_v1', 'HLT_Mu15_v2',
    'HLT_IsoMu17_v5', 'HLT_IsoMu17_v6', 'HLT_IsoMu17_v8', 'HLT_IsoMu17_v9', 'HLT_IsoMu17_v11',
    'HLT_Mu24_v1', 'HLT_Mu24_v2'
)

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
    process, hltProcess = HLTprocessName, isMC = isMC, applyZrecoilCorrection = False, runSVfit = False)
#--------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------------
#                                   Put the correct jet energy correction
#------------------------------------------------------------------------------------------------------------------------
#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#from CondCore.DBCommon.CondDBSetup_cfi import *
process.load('CondCore.DBCommon.CondDBSetup_cfi')
process.jec = cms.ESSource("PoolDBESSource",
    process.CondDBSetup,
    toGet = cms.VPSet(
      cms.PSet(record = cms.string("JetCorrectionsRecord"),
               tag = cms.string("JetCorrectorParametersCollection_Jec10V3_AK5Calo"),
               label=cms.untracked.string("AK5Calo")),
      cms.PSet(record = cms.string("JetCorrectionsRecord"),
               tag = cms.string("JetCorrectorParametersCollection_Jec10V3_AK5PF"),
               label=cms.untracked.string("AK5PF")),                                   
      cms.PSet(record = cms.string("JetCorrectionsRecord"),
               tag = cms.string("JetCorrectorParametersCollection_Jec10V3_AK5PFchs"),
               label=cms.untracked.string("AK5PF"))
    ),
    connect = cms.string('sqlite_fip:TauAnalysis/Configuration/data/Jec10V3.db')
    #connect = cms.string("frontier://FrontierPrep/CMS_COND_PHYSICSTOOLS")
)
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')
#-------------------------------------------------------------------------------------------------------------------------

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
    cms.untracked.vstring(
        'drop *_*_*_%s' % process.name_(),
        'keep edmMergeableCounter_*_*_*'
    )
)    

process.skimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    process.origFEVTSIMEventContent,
    process.tauIdEffSampleEventSelection,
    fileName = cms.untracked.string("testOutput/tauIdEffSample_RECO_relval.root")
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
    outputFileName = cms.string('testOutput/plotsZtoMuTau_tauIdEff_relval.root')
)

#-------------------------------------------------------------------------------------------------------------------------
#                                          Counting Events For self babysitting
#-------------------------------------------------------------------------------------------------------------------------
process.totalEventsProcessed = cms.EDProducer("EventCountProducer")
process.counterPath = cms.Path(process.totalEventsProcessed)
#-------------------------------------------------------------------------------------------------------------------------

process.o = cms.EndPath(process.skimOutputModule + process.saveZtoMuTau_tauIdEffPlots)

import TauAnalysis.Configuration.pathModifiers as pathModifiers

#Removes Sys*Up/Down collections, case insensitive
pathModifiers.PathRemover(process.p, ['sys', 'up'], True)
pathModifiers.PathRemover(process.p, ['sys', 'down'], True)

#--------------------------------------------------------------------------------
#
# CV: do **not** apply HLT trigger conditions to CMSSW_4_1_x/CMSSW_4_2_x MC,
#     to allow simulated events to be weighted by trigger efficiencies measured in Data instead
#
if isMC:
    process.commonSkimSequence.remove(process.hltMu)
#--------------------------------------------------------------------------------  

#--------------------------------------------------------------------------------
#
# CV: loosen Pt and eta cuts applied on tau-jet candidates
#
patPFTauSelectorsForTauIdEff = [
    "selectedPatPFTausFixedConeForTauIdEff",
    "selectedPatPFTausHPSForTauIdEff",
    "selectedPatPFTausHPSpTaNCForTauIdEff",
    "selectedPatPFTausShrinkingConeForTauIdEff"
]
for patPFTauSelectorForTauIdEff in patPFTauSelectorsForTauIdEff:
    patPFTauSelectorModule = getattr(process, patPFTauSelectorForTauIdEff)
    patPFTauSelectorModule.minJetPt = cms.double(15.0)
    patPFTauSelectorModule.maxJetEta = cms.double(2.5)
    patPFTauSelectorModule.applyECALcrackVeto = cms.bool(False)
#--------------------------------------------------------------------------------  
 
# define order in which different paths are run
process.schedule = cms.Schedule(
    process.counterPath,
    process.p,
    process.muonPFTauFixedConeSkimPath,
    process.muonPFTauShrinkingConeSkimPath,
    process.muonPFTauHPSskimPath,
    process.muonPFTauHPSpTaNCskimPath,
    process.o
)

processDumpFile = open('skimTauIdEffSample.dump' , 'w')
print >> processDumpFile, process.dumpPython()
