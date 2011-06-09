import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runZtoElecTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
#process.MessageLogger.suppressInfo = cms.untracked.vstring()
process.MessageLogger.suppressWarning = cms.untracked.vstring(
    "PATTriggerProducer",
    "PATElecTauPairProducer",
    "analyzeAHtoElecTauEventsOS",
    "analyzeAHtoElecTauEventsSS"
)
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START310_V4::All')

# import particle data table
# needed for print-out of generator level information
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoElecTauSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoElecTau_cff")
process.load("TauAnalysis.RecoTools.filterDataQuality_cfi")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeZtoElecTau_cff")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# print memory consumed by cmsRun
# (for debugging memory leaks)
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#    ignoreTotal = cms.untracked.int32(1) # default is one
#)
#
#process.printGenParticleList = cms.EDAnalyzer("ParticleListDrawer",
#    src = cms.InputTag("genParticles"),
#    maxEventsToPrint = cms.untracked.int32(100)
#)
#
# print debug information whenever plugins get loaded dynamically from libraries
# (for debugging problems with plugin related dynamic library loading)
#process.add_(cms.Service("PrintLoadingPlugins"))
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.saveZtoElecTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoElecTau.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_4_1_3/RelValZTT/GEN-SIM-RECO/START311_V2-v1/0037/286D4A6C-C651-E011-B6AC-001A92971BBA.root',
    '/store/relval/CMSSW_4_1_3/RelValZTT/GEN-SIM-RECO/START311_V2-v1/0038/18CCFD66-5A52-E011-A4BC-00304867918A.root'
  )
  #skipBadFiles = cms.untracked.bool(True)
)

#--------------------------------------------------------------------------------
# import utility function for configuring PAT trigger matching
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
switchOnTrigger(process, hltProcess = 'HLT', outputModule = '')
#process.patTrigger.addL1Algos = cms.bool(True)
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

# comment-out to take new HPS + TaNC combined tau id. algorithm
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
switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"), outputModule = '')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::METs
from TauAnalysis.Configuration.tools.metTools import *

# uncomment to add pfMET
# set Boolean swich to true in order to apply type-1 corrections
addPFMet(process, correct = False)

# uncomment to replace caloMET by pfMET in all di-tau objects
process.load("TauAnalysis.CandidateTools.diTauPairProductionAllKinds_cff")
replaceMETforDiTaus(process, cms.InputTag('patMETs'), cms.InputTag('patPFMETs'))
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#  do not produce momentum-corrected muons
from TauAnalysis.RecoTools.patLeptonSelection_cff import patMuonSelConfigurator
setattr(patMuonSelConfigurator, "src", "patMuons" )
process.selectPatMuons = patMuonSelConfigurator.configure(process = process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for changing cut values
from TauAnalysis.Configuration.tools.changeCut import changeCut

#
#  electron selection
#

# VBTF WP80 electron ID
changeCut(process,"selectedPatElectronsForElecTauId","(abs(superCluster.eta) < 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.004 & abs(deltaPhiSuperClusterTrackAtVtx) < 0.06 & hcalOverEcal < 0.04 & sigmaIetaIeta < 0.01) | (abs(superCluster.eta) > 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.007 & abs(deltaPhiSuperClusterTrackAtVtx) <0.03 & hcalOverEcal < 0.025 & sigmaIetaIeta < 0.03)")

#  electron eta range
changeCut(process,"selectedPatElectronsForElecTauEta","abs(eta) < 2.1")

#  electron pt
changeCut(process,"selectedPatElectronsForElecTauPt","pt > 18")

#  PF combinded relative isolation
changeCut(process,"selectedPatElectronsForElecTauIso",cms.double(0.08),"sumPtMaxEB")
changeCut(process,"selectedPatElectronsForElecTauIso",cms.double(0.04),"sumPtMaxEE")

changeCut(process,"selectedPatElectronsForElecTauIsoLooseIsolation",cms.double(0.25),"sumPtMax")

#  electron conversion veto
#  set deltaCot(theta) < 0.02 for real conversions
changeCut(process,"selectedPatElectronsForElecTauConversionVeto",cms.double(0), attribute = "nConvPairMax")
changeCut(process,"selectedPatElectronsForElecTauConversionVeto",cms.double(0.02), attribute = "cotThetaMax")
changeCut(process,"selectedPatElectronsForElecTauConversionVeto",cms.double(0.02), attribute = "docaElecTrackMax")
changeCut(process,"selectedPatElectronsForElecTauConversionVeto",cms.bool(True), attribute = "usePogMethod")
changeCut(process,"selectedPatElectronsForElecTauConversionVeto",cms.bool(True), attribute = "doMissingHitsCut")
changeCut(process,"selectedPatElectronsForElecTauConversionVeto",cms.bool(False), attribute = "doPixCut")

changeCut(process,"selectedPatElectronsForElecTauConversionVetoLooseIsolation",cms.double(2), attribute = "nConvPairMax")
changeCut(process,"selectedPatElectronsForElecTauConversionVetoLooseIsolation",cms.double(0.02), attribute = "cotThetaMax")
changeCut(process,"selectedPatElectronsForElecTauConversionVetoLooseIsolation",cms.double(0.02), attribute = "docaElecTrackMax")
changeCut(process,"selectedPatElectronsForElecTauConversionVetoLooseIsolation",cms.bool(True), attribute = "usePogMethod")
changeCut(process,"selectedPatElectronsForElecTauConversionVetoLooseIsolation",cms.bool(True), attribute = "doMissingHitsCut")
changeCut(process,"selectedPatElectronsForElecTauConversionVetoLooseIsolation",cms.bool(False), attribute = "doPixCut")

# upper limit on tranverse impact parameter of electron track 
changeCut(process, "selectedPatElectronsForElecTauTrkIP", 0.02, attribute = "IpMax")
changeCut(process, "selectedPatElectronsForElecTauTrkIPlooseIsolation", 0.02, attribute = "IpMax")

#
# hadronic tau decay selection
#

#  eta cut for taus
changeCut(process,"selectedPatTausForElecTauEta","abs(eta) < 2.3")
        
#  Pt cut for taus
changeCut(process,"selectedPatTausForElecTauPt","pt > 20")

# remove tau ID lead track pt discriminant and add decay mode finding
changeCut(process,"selectedPatTausLeadTrk",'tauID("decayModeFinding") > -0.5')
changeCut(process,"selectedPatTausLeadTrkPt",'tauID("decayModeFinding") > 0.5')
changeCut(process,"selectedPatTausForElecTauLeadTrk",'tauID("decayModeFinding") > -0.5')
changeCut(process,"selectedPatTausForElecTauLeadTrkPt",'tauID("decayModeFinding") > 0.5')

# change tau ID to HPS loose
changeCut(process,"selectedPatTausTaNCdiscr","tauID('byHPSloose') > 0.5")
changeCut(process,"selectedPatTausForElecTauTaNCdiscr","tauID('byHPSloose') > 0.5")

#  1/3-prong track cut for taus
changeCut(process,"selectedPatTausForElecTauProng","signalPFChargedHadrCands.size() = 1 | signalPFChargedHadrCands.size() = 3")

#  charge = +/-1 cut for taus
changeCut(process,"selectedPatTausForElecTauCharge","abs(charge) > 0.5 & abs(charge) < 1.5")

#  electron veto for taus
changeCut(process,"selectedPatTausForElecTauElectronVeto","tauID('againstElectronTight') > 0.5")
changeCut(process,"selectedPatTausElectronVeto","tauID('againstElectronTight') > 0.5")

# ECAl crack veto for taus 
changeCut(process,"selectedPatTausForElecTauEcalCrackVeto",'abs(eta) < 1.460 | abs(eta) > 1.558')

#  muon veto for taus
changeCut(process,"selectedPatTausForElecTauMuonVeto",'tauID("againstMuonTight") > 0.5')


#
# di-tau pair selection
#

# change lower limit on separation required between electron and tau-jet to dR > 0.5
#
# NOTE: this cut is NOT part of the EWK-10-013 analysis
#
changeCut(process, "selectedElecTauPairsAntiOverlapVeto", "dR12 > 0.5")
changeCut(process, "selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation", "dR12 > 0.5")

# change upper limit on electron + MET transverse mass 
changeCut(process, "selectedElecTauPairsMt1MET", "mt1MET < 40.")
changeCut(process, "selectedElecTauPairsMt1METlooseElectronIsolation", "mt1MET < 40.")

# turn off Pzeta cut
changeCut(process, "selectedElecTauPairsPzetaDiff", "(pZeta - 1.5*pZetaVis) > -1000.")
changeCut(process, "selectedElecTauPairsPzetaDiffLooseElectronIsolation", "(pZeta - 1.5*pZetaVis) > -1000.")

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
	src = cms.InputTag("isRecZtoElecTau")
)

process.p = cms.Path(
   process.producePatTupleZtoElecTauSpecific
# + process.printGenParticleList # uncomment to enable print-out of generator level particles
# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
  + process.selectZtoElecTauEvents
  + process.analyzeZtoElecTauSequence
  + process.saveZtoElecTauPlots
  + process.isRecZtoElecTau
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
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runZtoElecTau
#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#factorization#
##enableFactorization_runZtoElecTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for applyting Z-recoil corrections to MET
from TauAnalysis.Configuration.tools.mcToDataCorrectionTools import applyZrecoilCorrection_runZtoElecTau
##applyZrecoilCorrection_runZtoElecTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for disabling estimation of systematic uncertainties
from TauAnalysis.Configuration.tools.sysUncertaintyTools import enableSysUncertainties_runZtoElecTau
#
# define "hook" for keeping enabled/disabling estimation of systematic uncertainties
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#systematics#
##enableSysUncertainties_runZtoElecTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# disable event-dump output
# in order to reduce size of log-files
if hasattr(process, "disableEventDump"):
    process.analyzeZtoElecTauEventsOS.eventDumps = cms.VPSet()
    process.analyzeZtoElecTauEventsSS.eventDumps = cms.VPSet()
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# disable accessing generator level information
# if running on data
#from TauAnalysis.Configuration.tools.switchToData import switchToData
#switchToData(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleZtoElecTauSpecific)
#
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
#if not hasattr(process, "isBatchMode"):
#    process.p.replace(process.producePatTupleZtoElecTauSpecific, process.producePatTuple + process.producePatTupleZtoElecTauSpecific)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
