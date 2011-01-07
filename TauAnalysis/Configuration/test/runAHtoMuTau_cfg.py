import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runAHtoMuTau')

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.FwkReport.reportEvery = 1
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
#process.MessageLogger.suppressInfo = cms.untracked.vstring()
process.MessageLogger.suppressWarning = cms.untracked.vstring(
    "PATTriggerProducer",
    # Supress warnings in DiTau hist manager
    "analyzeAHtoMuTauEventsOS_woBtag",
    "analyzeAHtoMuTauEventsOS_wBtag",
    "analyzeAHtoMuTauEventsSS_woBtag",
    "analyzeAHtoMuTauEventsSS_wBtag"
)

process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START38_V14::All')

# import particle data table
# needed for print-out of generator level information
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleAHtoMuTauSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectAHtoMuTau_cff")
process.load("TauAnalysis.RecoTools.filterDataQuality_cfi")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeAHtoMuTau_cff")

#--------------------------------------------------------------------------------
# print memory consumed by cmsRun
# (for debugging memory leaks)
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#    ignoreTotal = cms.untracked.int32(1) # default is one
#)

process.printGenParticleList = cms.EDAnalyzer("ParticleListDrawer",
    src = cms.InputTag("genParticles"),
    maxEventsToPrint = cms.untracked.int32(2)
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

process.saveAHtoMuTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string("plotsAHtoMuTau.root")
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/data1/veelken/CMSSW_3_6_x/skims/Ztautau_1_1_sXK.root'
        #'file:/data1/friis/Run17/get_events/data_Mu_Run2010A_Sep17ReReco_pickevents/final_events.root'
        #'file:/data1/veelken/CMSSW_3_8_x/skims/test/mcDYttPU156bx_GEN_SIM_RECO_1_1_1VV.root'
        'file:/data1/veelken/CMSSW_3_8_x/skims/test/mcWplusJetsPU156bx_1_1_LO9.root'
    )
    #skipBadFiles = cms.untracked.bool(True)
)

#--------------------------------------------------------------------------------
# import utility function for configuring PAT trigger matching
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
switchOnTrigger(process, hltProcess = 'HLT', outputModule = '')
process.patTrigger.addL1Algos = cms.bool(True)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for switching pat::Tau input
# to different reco::Tau collection stored on AOD
import PhysicsTools.PatAlgos.tools.tauTools as tauTools

# comment-out to take reco::CaloTaus instead of reco::PFTaus
# as input for pat::Tau production
#tauTools.switchToCaloTau(process)

# comment-out to take shrinking dR = 5.0/Et(PFTau) signal cone
# instead of fixed dR = 0.07 signal cone reco::PFTaus
# as input for pat::Tau production
#tauTools.switchToPFTauShrinkingCone(process)
#tauTools.switchToPFTauFixedCone(process)

# comment-out to take new HPS + TaNC combined tau id. algorithm
tauTools.switchToPFTauHPSpTaNC(process)

# disable preselection on of pat::Taus
# (disabled also in TauAnalysis/RecoTools/python/patPFTauConfig_cfi.py ,
#  but re-enabled after switching tau collection)
process.cleanPatTaus.preselection = cms.string('')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::Jets
import PhysicsTools.PatAlgos.tools.jetTools as jetTools

# uncomment to replace caloJets by pfJets
jetTools.switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"),
             		     doBTagging = True, outputModule = '')
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

_MUON_ISO = 0.10
_LOOSE_MUON_ISO = 0.30

cut_values = {
    # Normal is final cuts
    'normal' : {
        'muon_pt' : 15,
        'muon_eta' : 2.1,
        'muon_iso' : _MUON_ISO, # rel. iso threshold
        'mt1MET' : 50.,
        'tau_pt' : 20,
        'tau_eta' : 2.3,
        'pzeta' : -20.,
        #'tanc' : "tauID('byTaNCloose') > 0.5",
        'tanc' : "tauID('byHPSloose') > 0.5",
        #'tanc' : "tauID('byTaNCmedium') > 0.5",
        'charge' : 'charge = 0',
    },
    # Loose loosens everything but muon Iso
    'loose' : {
        'muon_pt' : 13.5,
        'muon_eta' : 2.2,
        'muon_iso' : _MUON_ISO,
        'mt1MET' : 60.,
        'tau_pt' : 18,
        'tau_eta' : 2.4,
        'pzeta' : -25.,
        'tanc' : "tauID('byTaNCloose') > -1.", # turn off
        'charge' : 'charge > -1000',
    },
}

cut_values['loose_tauID'] = copy.deepcopy(cut_values['normal'])
cut_values['loose_tauID']['tanc'] = cut_values['loose']['tanc']

cut_values['loose_looseMuon_tightTau'] = copy.deepcopy(cut_values['loose'])
cut_values['loose_looseMuon_tightTau']['muon_iso'] = _LOOSE_MUON_ISO
cut_values['loose_looseMuon_tightTau']['tanc'] = "tauID('byTaNCloose') > 0.5 | tauID('byHPSloose') > 0.5"

# Loose tauid cuts
#cuts = cut_values['loose_tauID']
# Normal cuts
cuts = cut_values['normal']
# Loose cuts for skim
#cuts = cut_values['loose']
# Loose cuts to enable factorization
#cuts = cut_values['loose_looseMuon_tightTau']

# import utility function for changing cut values
from TauAnalysis.Configuration.tools.changeCut import changeCut

# change muon Pt threshold to 15 GeV
changeCut(process, "selectedPatMuonsPt15", "pt > %0.2f" % cuts['muon_pt'])

# change muon eta acceptance
changeCut(process, "selectedPatMuonsEta21", "abs(eta) < %0.2f"
          % cuts['muon_eta'])

# change tau pt threshold
changeCut(process, "selectedPatTausForMuTauPt20", "pt > %0.2f"
          % cuts['tau_pt'])

# change eta acceptance for tau-jets to |eta| < 2.3
changeCut(process, "selectedPatTausForMuTauEta23",
          "abs(eta) < %0.2f" % cuts['tau_eta'])

# Use absolute muon isolation
process.selectedPatMuonsPFRelIso.chargedHadronIso.ptMin = 1.0
process.selectedPatMuonsPFRelIso.neutralHadronIso.ptMin = 1.0
process.selectedPatMuonsPFRelIso.photonIso.ptMin = 1.0
process.selectedPatMuonsPFRelIso.sumPtMax = cuts['muon_iso']
process.selectedPatMuonsPFRelIso.sumPtMethod = "relative"

process.selectedPatMuonsPFRelIsoLooseIsolation.chargedHadronIso.ptMin = 1.0
process.selectedPatMuonsPFRelIsoLooseIsolation.neutralHadronIso.ptMin = 1.0
process.selectedPatMuonsPFRelIsoLooseIsolation.photonIso.ptMin = 1.0
process.selectedPatMuonsPFRelIsoLooseIsolation.sumPtMax = _LOOSE_MUON_ISO
process.selectedPatMuonsPFRelIsoLooseIsolation.sumPtMethod = "relative"

# change upper limit on muon + MET transverse mass to 50 GeV
changeCut(process, "selectedMuTauPairsMt1MET",
          "mt1MET < %0.2f" % cuts['mt1MET'])
changeCut(process, "selectedMuTauPairsMt1METlooseMuonIsolation",
          "mt1MET < %0.2f" % cuts['mt1MET'])

# change upper limit on muon + MET transverse mass to 50 GeV
changeCut(process, "selectedMuTauPairsForAHtoMuTauMt1MET",
          "mt1MET < %0.2f" % cuts['mt1MET'])
changeCut(process, "selectedMuTauPairsForAHtoMuTauMt1METlooseMuonIsolation",
          "mt1MET < %0.2f" % cuts['mt1MET'])

changeCut(process, "selectedMuTauPairsPzetaDiff",
          '(pZeta - 1.5*pZetaVis) > %0.2f' % cuts['pzeta'])
changeCut(process, "selectedMuTauPairsPzetaDiffLooseMuonIsolation",
          '(pZeta - 1.5*pZetaVis) > %0.2f' % cuts['pzeta'])

changeCut(process, "selectedMuTauPairsForAHtoMuTauPzetaDiff",
          '(pZeta - 1.5*pZetaVis) > %0.2f' % cuts['pzeta'])
changeCut(process, "selectedMuTauPairsForAHtoMuTauPzetaDiffLooseMuonIsolation",
          '(pZeta - 1.5*pZetaVis) > %0.2f' % cuts['pzeta'])

# set charge requirement - can be turned off to keep SS ditaus
changeCut(process, "selectedMuTauPairsZeroCharge", cuts['charge'])
changeCut(process, "selectedMuTauPairsForAHtoMuTauZeroCharge", cuts['charge'])
changeCut(process, "selectedMuTauPairsZeroChargeLooseMuonIsolation", cuts['charge'])
changeCut(process, "selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolation", cuts['charge'])

# change upper limit on tranverse impact parameter of muon track to 2mm
changeCut(process, "selectedPatMuonsTrkIP", 0.2, attribute = "IpMax")

# change cut on TaNC output in case using new HPS + TaNC combined tau id. algorithm
changeCut(process, "selectedPatTausTaNCdiscr", cuts['tanc'])
changeCut(process, "selectedPatTausForMuTauTaNCdiscr", cuts['tanc'])

# disable calorimeter muon veto for now...
changeCut(process, "selectedPatTausForMuTauCaloMuonVeto", "tauID('againstCaloMuon') > -1.")

# change lower limit on separation required between muon and tau-jet to dR > 0.5
changeCut(process, "selectedMuTauPairsAntiOverlapVeto", "dR12 > 0.5")
changeCut(process, "selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation", "dR12 > 0.5")

# Turn off lead track cut, hps doesn't use it.
changeCut(process, "selectedPatTausLeadTrkPt", 'tauID("leadingTrackPtCut") > -1e3')
changeCut(process, "selectedPatTausForMuTauLeadTrkPt", 'tauID("leadingTrackPtCut") > -1e3')

# disable b-tagging for now
# (--> all events will pass CentralJetVeto/fail CentralJetBtag selection)
#changeCut(process, "selectedPatJetsForAHtoMuTauBtag", "bDiscriminator('trackCountingHighEffBJetTags') < -1000.")
#--------------------------------------------------------------------------------

# Define a generic end path that filters the final events that a pool
# output module can be hooked into if desired.
process.filterFinalEvents = cms.EDFilter("BoolEventFilter",
    src = cms.InputTag("isRecAHtoMuTau")
)
process.p = cms.Path(
   process.producePatTupleAHtoMuTauSpecific
  #+ process.printEventContent
# + process.printGenParticleList # uncomment to enable print-out of generator level particles
# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
  + process.selectAHtoMuTauEvents
  + process.analyzeAHtoMuTauSequence
  + process.saveAHtoMuTauPlots
  + process.isRecAHtoMuTau
  + process.filterFinalEvents
)

process.q = cms.Path(process.dataQualityFilters)

# Dummy do-nothing module to allow an empty path
process.dummy = cms.EDProducer("DummyModule")
# Path that option output modules can be hooked into
process.endtasks = cms.EndPath(process.dummy)

process.schedule = cms.Schedule(
    process.q,
    process.p,
    #process.o,
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
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runAHtoMuTau
#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#factorization#
##enableFactorization_runAHtoMuTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for applyting Z-recoil corrections to MET
from TauAnalysis.Configuration.tools.mcToDataCorrectionTools import applyZrecoilCorrection_runAHtoMuTau
##applyZrecoilCorrection_runAHtoMuTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for disabling estimation of systematic uncertainties
from TauAnalysis.Configuration.tools.sysUncertaintyTools import enableSysUncertainties_runAHtoMuTau
#
# define "hook" for keeping enabled/disabling estimation of systematic uncertainties
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#systematics#
##enableSysUncertainties_runAHtoMuTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# disable event-dump output
# in order to reduce size of log-files
process.disableEventDump = cms.PSet()
if hasattr(process, "disableEventDump"):
    process.analyzeAHtoMuTauEventsOS_woBtag.eventDumps = cms.VPSet()
    process.analyzeAHtoMuTauEventsSS_woBtag.eventDumps = cms.VPSet()
    process.analyzeAHtoMuTauEventsOS_wBtag.eventDumps = cms.VPSet()
    process.analyzeAHtoMuTauEventsSS_wBtag.eventDumps = cms.VPSet()
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# disable accessing generator level information
# if running on data
#from TauAnalysis.Configuration.tools.switchToData import switchToData
#switchToData(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleAHtoMuTauSpecific)
#
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
if not hasattr(process, "isBatchMode"):
    process.p.replace(process.producePatTupleAHtoMuTauSpecific, process.producePatTuple + process.producePatTupleAHtoMuTauSpecific)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()

