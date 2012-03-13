import FWCore.ParameterSet.Config as cms
import copy
import sys
sys.setrecursionlimit(10000)

process = cms.Process('runAHtoElecTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.MessageLogger.suppressWarning = cms.untracked.vstring(
	"PATTriggerProducer",
	"PATElecTauPairProducer",
# Supress warnings in DiTau hist manager
	"analyzeAHtoElecTauEventsOS_woBtag",
	"analyzeAHtoElecTauEventsOS_wBtag",
	"analyzeAHtoElecTauEventsSS_woBtag",
	"analyzeAHtoElecTauEventsSS_wBtag"
)
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
#process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START42_V11::All')

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleAHtoElecTauSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectAHtoElecTau_cff")
process.load("TauAnalysis.RecoTools.filterDataQuality_cfi")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeAHtoElecTau_cff")

#--------------------------------------------------------------------------------
# print memory consumed by cmsRun
# (for debugging memory leaks)
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#    ignoreTotal = cms.untracked.int32(1) # default is one
#)

process.printGenParticleList = cms.EDAnalyzer("ParticleListDrawer",
    src = cms.InputTag("genParticles"),
    maxEventsToPrint = cms.untracked.int32(100)
)


# print debug information whenever plugins get loaded dynamically from libraries
# (for debugging problems with plugin related dynamic library loading)
#process.add_( cms.Service("PrintLoadingPlugins") )
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

#process.savePatTuple = cms.OutputModule("PoolOutputModule",
#	patTupleEventContent,
#	fileName = cms.untracked.string('patTuple.root')
#)

process.saveAHtoElecTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsAHtoElecTau.root')
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/store/user/jkolb/VBF_HToTauTau_M-120_7TeV-powheg-pythia6-tauola/skimElecTau_428_Fall11_v1/6aa5d932edddb97c8f87b85a020d9993/elecTauSkim_1_1_yQv.root'
	)
    #skipBadFiles = cms.untracked.bool(True)    
)

HLTprocessName = "HLT" # use for 2011 Data
##HLTprocessName = "REDIGI311X" # use for Spring'11 reprocessed MC

#--------------------------------------------------------------------------------
# import utility function for switching pat::Tau input
# to different reco::Tau collection stored on AOD
from PhysicsTools.PatAlgos.tools.tauTools import * 

# comment-out to take reco::CaloTaus instead of reco::PFTaus
# as input for pat::Tau production
#switchToCaloTau(process)

# comment-out to take shrinking dR = 5.0/Et(PFTau) signal cone
# instead of fixed dR = 0.07 signal cone reco::PFTaus
# as input for pat::Tau production
#switchToPFTauShrinkingCone(process)
#switchToPFTauFixedCone(process)
#switchToPFTauHPSpTaNC(process)
switchToPFTauHPS(process)

# disable preselection on of pat::Taus
# (disabled also in TauAnalysis/RecoTools/python/patPFTauConfig_cfi.py ,
#  but re-enabled after switching tau collection)
process.cleanPatTaus.preselection = cms.string('')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *

# uncomment to replace caloJets by pfJets
switchJetCollection(process,
        jetCollection = cms.InputTag("ak5PFJets"),
        jetCorrLabel = ('AK5PF', ['L1FastJet', 'L2Relative', 'L3Absolute']),
        doBTagging = True,
        outputModule = "")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for configuring PAT trigger matching
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTriggerMatching, switchOnTrigger

# make trigger-matched collections of electrons and taus
#from PhysicsTools.PatAlgos.triggerLayer1.triggerMatcher_cfi import cleanElectronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoT
#process.cleanElectronTriggerMatchHLTElectronPlusTau = cleanElectronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoT.clone()
#process.cleanElectronTriggerMatchHLTElectronPlusTau.matchedCuts = cms.string( 'path( "HLT_Ele*_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_*IsoPFTau20_v*" )' )
# do matching
#switchOnTriggerMatching(process, triggerMatchers = [ 'cleanElectronTriggerMatchHLTElectronPlusTau' ], hltProcess = HLTprocessName, outputModule = '')

switchOnTrigger(process, hltProcess = HLTprocessName, outputModule = '')

#process.patTrigger.addL1Algos = cms.bool(True)
from TauAnalysis.Configuration.cfgOptionMethods import _setTriggerProcess
_setTriggerProcess(process, cms.InputTag("TriggerResults", "", HLTprocessName))
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::METs
import TauAnalysis.Configuration.tools.metTools as metTools

# uncomment to add pfMET
# set Boolean swich to true in order to apply type-1 corrections
metTools.addPFMet(process, correct = False)

# uncomment to replace caloMET by pfMET in all di-tau objects
process.load("TauAnalysis.CandidateTools.diTauPairProductionAllKinds_cff")
metTools.replaceMETforDiTaus(process, cms.InputTag('patMETs'), cms.InputTag('patPFMETs'))
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#  do not produce momentum-corrected muons
from TauAnalysis.RecoTools.patLeptonSelection_cff import patMuonSelConfigurator
setattr(patMuonSelConfigurator, "src", "cleanPatMuons" )
process.selectPatMuons = patMuonSelConfigurator.configure(process = process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#  make cut changes
from TauAnalysis.Configuration.tools.changeCut import changeCut
# update all tau discriminant to HPS types
changeCut(process,"selectedPatTausLeadTrk",'tauID("decayModeFinding")')
changeCut(process,"selectedPatTausLeadTrkPt",'tauID("decayModeFinding")')
changeCut(process,"selectedPatTausTaNCdiscr",'tauID("byLooseCombinedIsolationDeltaBetaCorr")')

#--------------------------------------------------------------------------------

# before starting to process 1st event, print event content
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")
process.filterFirstEvent = cms.EDFilter("EventCountFilter",
	numEvents = cms.int32(1)
)
process.n = cms.Path(process.filterFirstEvent + process.printEventContent)

process.o = cms.Path(process.dataQualityFilters)

# Define a generic end path that filters the final events that a pool
# output module can be hooked into if desired.
process.filterFinalEvents = cms.EDFilter("BoolEventFilter",
	src = cms.InputTag("isRecAHtoElecTau")
)

process.p = cms.Path(
	process.producePatTupleAHtoElecTauSpecific
	# + process.printGenParticleList # uncomment to enable print-out of generator level particles
	# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
	+ process.selectAHtoElecTauEvents 
	+ process.analyzeAHtoElecTauSequence
	+ process.saveAHtoElecTauPlots 
	+ process.isRecAHtoElecTau
	+ process.filterFinalEvents
)

# Dummy do-nothing module to allow an empty path
process.dummy = cms.EDProducer("DummyModule")
# Path that option output modules can be hooked into
process.endtasks = cms.EndPath(process.dummy)

process.schedule = cms.Schedule(
	process.n,
	process.o,
    process.p,
    process.endtasks
)
#process.options = cms.untracked.PSet(
#	wantSummary = cms.untracked.bool(True)
#)

#--------------------------------------------------------------------------------
# import utility function for switching HLT InputTags when processing
# RECO/AOD files produced by MCEmbeddingTool
from TauAnalysis.MCEmbeddingTools.tools.switchInputTags import switchInputTags
#
# comment-out to switch HLT InputTags
#switchInputTags(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runAHtoElecTau
#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#
#__#factorization#
##enableFactorization_runAHtoElecTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for applyting Z-recoil corrections to MET
from TauAnalysis.Configuration.tools.mcToDataCorrectionTools import applyZrecoilCorrection_runAHtoElecTau
##applyZrecoilCorrection_runAHtoElecTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for applyting electron trigger correction factor
from TauAnalysis.Configuration.tools.mcToDataCorrectionTools import applyElectronTriggerEfficiencyCorrection_runAHtoElecTau
#applyElectronTriggerEfficiencyCorrection_runAHtoElecTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# disable event-dump output
# in order to reduce size of log-files
#process.disableEventDump = cms.PSet()
if hasattr(process, "disableEventDump"):
	process.analyzeAHtoElecTauEventsOS_wBtag.eventDumps = cms.VPSet()
	process.analyzeAHtoElecTauEventsOS_woBtag.eventDumps = cms.VPSet()
	process.analyzeAHtoElecTauEventsSS_wBtag.eventDumps = cms.VPSet()
	process.analyzeAHtoElecTauEventsSS_woBtag.eventDumps = cms.VPSet()
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility to remove modules operating on GEN-level collections
from TauAnalysis.Configuration.tools.switchToData import *
#
# uncomment when running over DATA samples
##switchToData(process)#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for disabling estimation of systematic uncertainties
#
from TauAnalysis.Configuration.tools.sysUncertaintyTools import enableSysUncertainties_runAHtoElecTau
#
# define "hook" for keeping enabled/disabling estimation of systematic uncertainties
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#systematics#
##enableSysUncertainties_runAHtoElecTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# sequence to produce full PAT
#
process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleAHtoElecTauSpecific)
#
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
