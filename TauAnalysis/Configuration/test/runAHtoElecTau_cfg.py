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
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START311_V2::All')

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
        '/store/relval/CMSSW_4_2_3/RelValZTT/GEN-SIM-RECO/START42_V12-v2/0062/4CEA9C47-287B-E011-BAB7-00261894396B.root',
        '/store/relval/CMSSW_4_2_3/RelValZTT/GEN-SIM-RECO/START42_V12-v2/0066/B6B51325-DA7B-E011-9E95-0018F3D096C6.root'
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
switchToPFTauHPSpTaNC(process)
#switchToPFTauHPS(process)

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
process.patJetCorrections.remove(process.patJetCorrFactors)
process.producePatTupleZtoElecTauSpecific.remove(process.pfMEtType1and2corrected)
process.producePatTupleZtoElecTauSpecific.remove(process.patPFtype1METs)
process.producePatTupleAHtoElecTauSpecific.remove(process.pfMEtType1and2corrected)
process.producePatTupleAHtoElecTauSpecific.remove(process.patPFtype1METs)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for configuring PAT trigger matching
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
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
setattr(patMuonSelConfigurator, "src", "patMuons" )
process.selectPatMuons = patMuonSelConfigurator.configure(process = process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#  make cut changes
from TauAnalysis.Configuration.tools.changeCut import changeCut

#
# update vertex sources
#
#changeCut(process,"selectedPatMuonsVbTfId",cms.InputTag("selectedPrimaryVertexQuality"),"vertexSource")
#changeCut(process,"selectedPatMuonsTrkIP",cms.InputTag("selectedPrimaryVertexQuality"),"vertexSource")
#changeCut(process,"selectedPatElectronsTrkIP",cms.InputTag("selectedPrimaryVertexQuality"),"vertexSource")
#changeCut(process,"selectedPatElectronsTrkIPlooseIsolation",cms.InputTag("selectedPrimaryVertexQuality"),"vertexSource")
#changeCut(process,"selectedPatElectronsForElecTauTrkIP",cms.InputTag("selectedPrimaryVertexQuality"),"vertexSource")
#changeCut(process,"selectedPatElectronsForElecTauTrkIPlooseIsolation",cms.InputTag("selectedPrimaryVertexQuality"),"vertexSource")

#
# electron selection
#

#  VBTF WP80 electron ID (pt > 20); WP70 (15 < pt < 20)
changeCut(process,"selectedPatElectronsForElecTauId",'(abs(superCluster.eta) < 1.479 & pt > 20 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.004 & abs(deltaPhiSuperClusterTrackAtVtx) < 0.06 & hcalOverEcal < 0.04 & sigmaIetaIeta < 0.01) | (abs(superCluster.eta) > 1.479 & pt > 20 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.007 & abs(deltaPhiSuperClusterTrackAtVtx) <0.03 & hcalOverEcal < 0.025 & sigmaIetaIeta < 0.03) | (abs(superCluster.eta) < 1.479 & pt < 20 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.004 & abs(deltaPhiSuperClusterTrackAtVtx) < 0.03 & hcalOverEcal < 0.025 & sigmaIetaIeta < 0.01 & (fbrem > 0.15 | (abs(superCluster.eta) < 1 & eSuperClusterOverP > 0.95) )) | (abs(superCluster.eta) > 1.479 & pt < 20 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.005 & abs(deltaPhiSuperClusterTrackAtVtx) <0.02 & hcalOverEcal < 0.025 & sigmaIetaIeta < 0.03 & (fbrem > 0.15 | (abs(superCluster.eta) < 1 & eSuperClusterOverP > 0.95) ))')
#  electron anti-crack cut
changeCut(process,"selectedPatElectronsForElecTauAntiCrackCut","abs(eta) < 1.442 | abs(eta) > 1.566")

#  electron eta range
changeCut(process,"selectedPatElectronsForElecTauEta","abs(eta) < 2.1")

#  electron pt
changeCut(process,"selectedPatElectronsForElecTauPt","pt > 15")

#  PF relative iso
changeCut(process,"selectedPatElectronsForElecTauIso",cms.double(0.013),"sumPtMaxEB")
changeCut(process,"selectedPatElectronsForElecTauIso",cms.double(0.09),"sumPtMaxEE")
changeCut(process,"selectedPatElectronsForElecTauIsoLooseIsolation",cms.double(0.3),"sumPtMax")

#  electron conversion veto
changeCut(process,"selectedPatElectronsForElecTauConversionVeto",cms.int32(0), attribute = "maxMissingInnerHits")
changeCut(process,"selectedPatElectronsForElecTauConversionVetoLooseIsolation",cms.int32(2), attribute = "maxMissingInnerHits")
changeCut(process,"selectedPatElectronsForElecTauConversionVetoLooseIsolation",cms.bool(True), attribute = "invertConversionVeto")

#  electron track IP_xy cut
changeCut(process,"selectedPatElectronsForElecTauTrkIP",cms.double(0.045),"IpMax")
changeCut(process, "selectedPatElectronsForElecTauTrkIPlooseIsolation", 0.045, attribute = "IpMax")

#
# hadronic tau decay selection
#

#  eta cut for taus
changeCut(process,"selectedPatTausForElecTauEta","abs(eta) < 2.3")

#  Pt cut for taus
changeCut(process,"selectedPatTausForElecTauPt","pt > 20")

# decay mode finding
changeCut(process,"selectedPatTausLeadTrk",'tauID("decayModeFinding") > 0.5')
changeCut(process,"selectedPatTausForElecTauDecayModeFinding",'tauID("decayModeFinding") > 0.5')

# decay mode finding
changeCut(process,"selectedPatTausLeadTrkPt",'tauID("leadingTrackPtCut") > 0.5')
changeCut(process,"selectedPatTausForElecTauLeadTrkPt",'tauID("leadingTrackPtCut") > 0.5')

#  put tau ID at HPS loose
changeCut(process,"selectedPatTausTaNCdiscr",'tauID("byHPSloose") > 0.5')
changeCut(process,"selectedPatTausForElecTauTaNCdiscr",'tauID("byHPSloose") > 0.5')

#  electron veto for taus
changeCut(process,"selectedPatTausForElecTauElectronVeto",'tauID("againstElectronTight")')
process.hpsPFTauDiscriminationByTightElectronRejection.ApplyCut_EcalCrackCut = cms.bool(False)
process.hpsPFTauDiscriminationByTightElectronRejection.BremCombined_HOP = cms.double(0.08)

# ECAl crack veto for taus 
changeCut(process,"selectedPatTausForElecTauEcalCrackVeto",'abs(eta) < 1.460 | abs(eta) > 1.558')

#  muon veto for taus
changeCut(process,"selectedPatTausForElecTauMuonVeto",'tauID("againstMuonLoose") > 0.5')

#
# di-tau pair selection
#

#  elec/tau overlap cut
changeCut(process,"selectedElecTauPairsForAHtoElecTauAntiOverlapVeto","dR12 > 0.5")
changeCut(process,"selectedElecTauPairsForAHtoElecTauAntiOverlapVetoLooseElectronIsolation", "dR12 > 0.5")

#  transverse mass of electron + MET
changeCut(process,"selectedElecTauPairsForAHtoElecTauMt1MET","mt1MET < 4000.")
changeCut(process,"selectedElecTauPairsForAHtoElecTauMt1METlooseElectronIsolation", "mt1MET < 4000.")

# enable cut on Pzeta variable
changeCut(process, "selectedElecTauPairsForAHtoElecTauPzetaDiff", "(pZeta - 1.5*pZetaVis) > -20.")
changeCut(process, "selectedElecTauPairsForAHtoElecTauPzetaDiffLooseElectronIsolation", "(pZeta - 1.5*pZetaVis) > -20.")

# change isolation treshold for second electron used in di-electron veto to 0.30 * electron Pt
changeCut(process, "selectedPatElectronsForZeeHypothesesLoosePFRelIso", 0.30, "sumPtMax")

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
#
process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleAHtoElecTauSpecific)
#
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
#if not hasattr(process, "isBatchMode"):
#    process.p.replace(process.producePatTupleAHtoElecTauSpecific, process.producePatTuple + process.producePatTupleAHtoElecTauSpecific)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
