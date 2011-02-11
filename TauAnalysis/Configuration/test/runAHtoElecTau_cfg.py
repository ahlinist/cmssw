import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runAHtoElecTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.MessageLogger.suppressWarning = cms.untracked.vstring(
	"PATTriggerProducer",
# Supress warnings in DiTau hist manager
	"analyzeAHtoElecTauEventsOS_woBtag",
	"analyzeAHtoElecTauEventsOS_wBtag",
	"analyzeAHtoElecTauEventsSS_woBtag",
	"analyzeAHtoElecTauEventsSS_wBtag"
)
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START38_V12::All')

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

# print event content 
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")
process.filterFirstEvent = cms.EDFilter("EventCountFilter",
	numEvents = cms.int32(1)
)
process.o = cms.Path(process.filterFirstEvent + process.printEventContent)

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
		'/store/relval/CMSSW_3_8_7/RelValZTT/GEN-SIM-RECO/START38_V13-v1/0016/26155577-92FC-DF11-8E56-001A92810A9A.root',
		'/store/relval/CMSSW_3_8_7/RelValZTT/GEN-SIM-RECO/START38_V13-v1/0016/506F0476-92FC-DF11-8886-00304867C1BC.root',
		'/store/relval/CMSSW_3_8_7/RelValZTT/GEN-SIM-RECO/START38_V13-v1/0017/6262925F-9DFC-DF11-B9EF-0026189438BA.root',
		'/store/relval/CMSSW_3_8_7/RelValZTT/GEN-SIM-RECO/START38_V13-v1/0017/AA64EF7B-93FC-DF11-AB92-001A92971BC8.root'
	)
    #skipBadFiles = cms.untracked.bool(True)    
)


#--------------------------------------------------------------------------------
#  make cut changes
from TauAnalysis.Configuration.tools.changeCut import changeCut

#  VBTF WP80 electron ID
changeCut(process,"selectedPatElectronsForElecTauId","(abs(superCluster.eta) < 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.004 & abs(deltaPhiSuperClusterTrackAtVtx) < 0.06 & hcalOverEcal < 0.04 & sigmaIetaIeta < 0.01) | (abs(superCluster.eta) > 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.007 & abs(deltaPhiSuperClusterTrackAtVtx) <0.03 & hcalOverEcal < 0.025 & sigmaIetaIeta < 0.03)")

#  electron eta range
changeCut(process,"selectedPatElectronsForElecTauEta","abs(eta) < 2.1")

#  electron pt
changeCut(process,"selectedPatElectronsForElecTauPt","pt > 15")

#  PF relative iso
changeCut(process,"selectedPatElectronsForElecTauIso",cms.double(0.09),"sumPtMaxEB")
changeCut(process,"selectedPatElectronsForElecTauIso",cms.double(0.06),"sumPtMaxEE")

#  electron conversion veto
changeCut(process,"selectedPatElectronsForElecTauConversionVeto",cms.double(1),"nTrkMax")

#  electron track IP_xy cut
changeCut(process,"selectedPatElectronsForElecTauTrkIP",cms.double(0.05),"IpMax")


#  eta cut for taus
changeCut(process,"selectedPatTausForElecTauEta","abs(eta) < 2.3")

#  Pt cut for taus
changeCut(process,"selectedPatTausForElecTauPt","pt > 20")

#  put tau ID at HPS loose
changeCut(process,"selectedPatTausForElecTauTaNCdiscr",'tauID("byHPSloose") > 0.5')

#  1/3-prong track cut for taus
changeCut(process,"selectedPatTausForElecTauProng","signalPFChargedHadrCands.size() = 1 | signalPFChargedHadrCands.size() = 3")

#  charge = +/-1 cut for taus
changeCut(process,"selectedPatTausForElecTauCharge","abs(charge) > 0.5 & abs(charge) < 1.5")

#  muon veto for taus
changeCut(process,"selectedPatTausForElecTauCharge",'tauID("againstMuon") > 0.5')

#  electron veto for taus
changeCut(process,"selectedPatTausForElecTauCharge","leadPFCand().isNonnull() & leadPFCand().mva_e_pi() < 0.6")


#  elec/tau overlap cut
changeCut(process,"selectedElecTauPairsAntiOverlapVeto","dR12 > 0.5")

#  transverse mass of electron + MET
changeCut(process,"selectedElecTauPairsMt1MET","mt1MET < 40.")


#--------------------------------------------------------------------------------

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
switchToPFTauHPSpTaNC(process)

# disable preselection on of pat::Taus
# (disabled also in TauAnalysis/RecoTools/python/patPFTauConfig_cfi.py ,
#  but re-enabled after switching tau collection)
process.cleanPatTaus.preselection = cms.string('')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *

# uncomment to replace caloJets by pfJets
switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"), 
		doBTagging = True, outputModule = "")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for configuring PAT trigger matching
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
switchOnTrigger(process, hltProcess = 'HLT', outputModule = '')
#switchOnTrigger(process, hltProcess = 'REDIGI38X', outputModule = '')
process.patTrigger.addL1Algos = cms.bool(True)
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
setattr(patMuonSelConfigurator, "src", "patMuons" )
process.selectPatMuons = patMuonSelConfigurator.configure(process = process)
#--------------------------------------------------------------------------------

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

process.q = cms.Path(process.dataQualityFilters)

process.o = cms.Path(process.printEventContent)

# Dummy do-nothing module to allow an empty path
process.dummy = cms.EDProducer("DummyModule")
# Path that option output modules can be hooked into
process.endtasks = cms.EndPath(process.dummy)

process.schedule = cms.Schedule(
	#process.q,
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
process.disableEventDump = cms.PSet()
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
#
process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleAHtoElecTauSpecific)
#
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
if not hasattr(process, "isBatchMode"):
    process.p.replace(process.producePatTupleAHtoElecTauSpecific, process.producePatTuple + process.producePatTupleAHtoElecTauSpecific)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
