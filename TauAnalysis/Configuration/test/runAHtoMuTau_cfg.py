import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runAHtoMuTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.FwkReport.reportEvery = 1
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
#process.MessageLogger.suppressInfo = cms.untracked.vstring()
process.MessageLogger.suppressWarning = cms.untracked.vstring("PATTriggerProducer",)
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
#process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START38_V12::All')

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

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_cfi import *
#--------------------------------------------------------------------------------

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

# print debug information whenever plugins get loaded dynamically from libraries
# (for debugging problems with plugin related dynamic library loading)
#process.add_( cms.Service("PrintLoadingPlugins") )
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.saveAHtoMuTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsAHtoMuTau.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0021/F405BC9A-525D-DF11-AB96-002618943811.root',
        #'/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0020/EE3E8F74-365D-DF11-AE3D-002618FDA211.root'
        'file:/data1/veelken/CMSSW_3_6_x/skims/Ztautau_1_1_sXK.root'
    )
    #skipBadFiles = cms.untracked.bool(True)
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.analyzeAHtoMuTauEvents_woBtag.filters[0] = copy.deepcopy(#genPhaseSpaceCut#)
#__process.analyzeAHtoMuTauEvents_wBtag.filters[0] = copy.deepcopy(#genPhaseSpaceCut#)
#__process.saveAHtoMuTauPlots.outputFileName = #plotsOutputFileName#
#__#isBatchMode#
#__#disableEventDump#
#__process.GlobalTag.globaltag = '#globalTag#'
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for configuring PAT trigger matching
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
switchOnTrigger(process, hltProcess = 'HLT', outputModule = '')
process.patTrigger.addL1Algos = cms.bool(True)
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

# disable preselection on of pat::Taus
# (disabled also in TauAnalysis/RecoTools/python/patPFTauConfig_cfi.py ,
#  but re-enabled after switching tau collection)
process.cleanPatTaus.preselection = cms.string('')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *

# uncomment to replace caloJets by pfJets
##switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"))
process.patJets.addDiscriminators = False
process.patJets.addTagInfos = False
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
# import utility function for changing cut values
from TauAnalysis.Configuration.tools.changeCut import changeCut

# change muon Pt threshold to 15 GeV
changeCut(process, "selectedPatMuonsPt10", "pt > 15.")

# disable cut on muon calo. + segment compatibility
# (check that muon calo. compatibility is not affected by pile-up before re-enabling this cut)
changeCut(process, "selectedPatMuonsPionVeto", -1000., attribute = "AntiPionCut")
changeCut(process, "selectedPatMuonsPionVetoLooseIsolation", -1000., attribute = "AntiPionCut")

# change upper limit on tranverse impact parameter of muon track to 2mm
changeCut(process, "selectedPatMuonsTrkIP", 0.2, attribute = "IpMax")

# change eta acceptance for tau-jets to |eta| < 2.3
changeCut(process, "selectedPatTausForMuTauEta21", "abs(eta) < 2.3")

# disable cuts on tau id. discriminators for Track && ECAL isolation
changeCut(process, "selectedPatTausForMuTauTrkIso", "tauID('trackIsolation') > -1.")
changeCut(process, "selectedPatTausForMuTauEcalIso", "tauID('ecalIsolation') > -1.")

# change cut on TaNC output in case using new HPS + TaNC combined tau id. algorithm
# CV: discriminators by Track/ECAL isolation do not exist for the combined tau id. algorithm
#     and need to be replaced by dummy cuts, in order to avoid run-time exceptions
changeCut(process, "selectedPatTausTrkIso", "tauID('byTaNCtight') > -1.")
changeCut(process, "selectedPatTausForMuTauTrkIso", "tauID('byTaNCtight') > -1.")
changeCut(process, "selectedPatTausEcalIso", "tauID('byTaNCtight') > -1.")
changeCut(process, "selectedPatTausForMuTauEcalIso", "tauID('byTaNCtight') > -1.")
changeCut(process, "selectedPatTausTaNCdiscr", "tauID('byTaNCtight') > 0.5")
changeCut(process, "selectedPatTausForMuTauTaNCdiscr", "tauID('byTaNCtight') > 0.5")

# change lower limit on separation required between muon and tau-jet to dR > 0.5
changeCut(process, "selectedMuTauPairsAntiOverlapVeto", "dR12 > 0.5")
changeCut(process, "selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation", "dR12 > 0.5")

# change upper limit on muon + MET transverse mass to 40 GeV
changeCut(process, "selectedMuTauPairsMt1MET", "mt1MET < 40.")
changeCut(process, "selectedMuTauPairsMt1METlooseMuonIsolation", "mt1MET < 40.")

# disable b-tagging for now
# (--> all events will pass CentralJetVeto/fail CentralJetBtag selection)
changeCut(process, "selectedPatJetsForAHtoMuTauBtag", "bDiscriminator('trackCountingHighEffBJetTags') < -1000.")
#--------------------------------------------------------------------------------

# disable tau-ID to create reduced data sample
changeCut(process, "selectedPatTausForMuTauTaNCdiscr", 'tauID("byTaNCtight") > -1.e3')

# disable charge cut to create reduced data sample
changeCut(process, "selectedMuTauPairsZeroCharge", 'charge > -10000')

# Absolute muon isolation
process.selectedPatMuonsPFRelIso.chargedHadronIso.ptMin = 1.0
process.selectedPatMuonsPFRelIso.neutralHadronIso.ptMin = 2000.
process.selectedPatMuonsPFRelIso.photonIso.ptMin = 1.5
process.selectedPatMuonsPFRelIso.sumPtMax = 1.0
process.selectedPatMuonsPFRelIso.sumPtMethod = "absolute"

process.p = cms.Path(
   process.producePatTupleAHtoMuTauSpecific
# + process.printGenParticleList # uncomment to enable print-out of generator level particles
# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
  + process.selectAHtoMuTauEvents
  + process.analyzeAHtoMuTauEvents
  + process.saveAHtoMuTauPlots
)

process.q = cms.Path(process.dataQualityFilters)

# Define a generic end path that filters the final events that a pool
# output module can be hooked into if desired.
process.filterFinalEvents = cms.EDFilter(
    "BoolEventFilter",
    src = cms.InputTag("isRecAHtoMuTauCentralJetBtag"),
)
# Path that will pass/fail depending on if the event passed.
process.selectFinalEvents = cms.Path(process.isRecAHtoMuTauCentralJetBtag
                                     *process.filterFinalEvents)
# Dummy do-nothing module to allow an empty path
process.dummy = cms.EDProducer("DummyModule")
# Path that option output modules can be hooked into
process.endtasks = cms.EndPath(process.dummy)

process.schedule = cms.Schedule(process.q, process.p, process.selectFinalEvents,
                                process.endtasks)

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
##process.disableEventDump = cms.PSet()
if hasattr(process, "disableEventDump"):
    process.analyzeAHtoMuTauEvents_woBtag.eventDumps = cms.VPSet()
    process.analyzeAHtoMuTauEvents_wBtag.eventDumps = cms.VPSet()

process.eventDumps = [process.analyzeAHtoMuTauEvents_woBtag,
                      process.analyzeAHtoMuTauEvents_wBtag]
# Make it easy for the cfg option setter
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

