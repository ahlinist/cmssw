import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runZtoMuTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
#process.MessageLogger.suppressInfo = cms.untracked.vstring()
process.MessageLogger.suppressWarning = cms.untracked.vstring("PATTriggerProducer",)
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
process.load("TauAnalysis.Configuration.producePatTupleZtoMuTauSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoMuTau_cff")
process.load("TauAnalysis.RecoTools.filterDataQuality_cfi")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeZtoMuTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_10TeV_cfi import *
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

process.saveZtoMuTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoMuTau.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0021/F405BC9A-525D-DF11-AB96-002618943811.root',
        #'/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0020/EE3E8F74-365D-DF11-AE3D-002618FDA211.root'
        #'file:/data1/veelken/CMSSW_3_6_x/skims/Ztautau_1_1_sXK.root'
        #'file:/data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/selEvents_AHtoMuTau_woBtag_runs145762to148058_RECO.root'
        #'file:/data1/veelken/CMSSW_3_8_x/skims/test/mcDYttPU156bx_GEN_SIM_RECO_1_1_1VV.root'
        #'file:/data1/friis/PickMikesEvents/mikes_events_2010a.root',
        #'file:/data1/friis/PickMikesEvents/mikes_events_2010a001.root',
        #'file:/data1/friis/PickMikesEvents/mikes_events_2010b.root',
        #'file:/data1/friis/PickMikesEvents/mikes_events_2010b001.root',
        #'file:/data1/friis/PickMikesEvents/mikes_events_2010b002.root'
        #'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/skims/ZtoMuTau/test/Ztautau_1_1_sXK.root'
        #'file:/tmp/veelken/Ztautau_1_1_sXK.root'
        'file:/data1/veelken/tmp/final_events_AHtoMuTau_ZtautauPOWHEG_Run32_51_0_ZW7.root'
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
# import utility function for changing cut values
from TauAnalysis.Configuration.tools.changeCut import changeCut

# change upper limit on tranverse impact parameter of muon track to 2mm
changeCut(process, "selectedPatMuonsTrkIP", 0.2, attribute = "IpMax")

# change cut on TaNC output in case using new HPS + TaNC combined tau id. algorithm
changeCut(process, "selectedPatTausTaNCdiscr", "tauID('byTaNCloose') > 0.5")
changeCut(process, "selectedPatTausForMuTauTaNCdiscr", "tauID('byTaNCloose') > 0.5")

# disable calorimeter muon veto for now...
changeCut(process, "selectedPatTausForMuTauCaloMuonVeto", "tauID('againstCaloMuon') > -1.")

# change lower limit on separation required between muon and tau-jet to dR > 0.5
changeCut(process, "selectedMuTauPairsAntiOverlapVeto", "dR12 > 0.5")
changeCut(process, "selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation", "dR12 > 0.5")

# change upper limit on muon + MET transverse mass to 50 GeV
changeCut(process, "selectedMuTauPairsMt1MET", "mt1MET < 50.")
changeCut(process, "selectedMuTauPairsMt1METlooseMuonIsolation", "mt1MET < 50.")

# enable cut on Pzeta variable
changeCut(process, "selectedMuTauPairsPzetaDiff", "(pZeta - 1.5*pZetaVis) > -20.")
changeCut(process, "selectedMuTauPairsPzetaDiffLooseMuonIsolation", "(pZeta - 1.5*pZetaVis) > -20.")
#--------------------------------------------------------------------------------

# before starting to process 1st event, print event content
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")
process.filterFirstEvent = cms.EDFilter("EventCountFilter",
    numEvents = cms.int32(1)
)

process.o = cms.Path(process.filterFirstEvent + process.printEventContent)

process.p = cms.Path(
   process.producePatTupleZtoMuTauSpecific
# + process.printGenParticleList # uncomment to enable print-out of generator level particles
# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
  + process.selectZtoMuTauEvents
  + process.analyzeZtoMuTauSequence
  + process.saveZtoMuTauPlots
)

process.q = cms.Path(process.dataQualityFilters)

process.schedule = cms.Schedule(process.o, process.q, process.p)

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
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runZtoMuTau
#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#factorization#
##enableFactorization_runZtoMuTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for applyting Z-recoil corrections to MET
from TauAnalysis.Configuration.tools.mcToDataCorrectionTools import applyZrecoilCorrection_runZtoMuTau
##applyZrecoilCorrection_runZtoMuTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for disabling estimation of systematic uncertainties
from TauAnalysis.Configuration.tools.sysUncertaintyTools import enableSysUncertainties_runZtoMuTau
#
# define "hook" for keeping enabled/disabling estimation of systematic uncertainties
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#systematics#
##enableSysUncertainties_runZtoMuTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# disable event-dump output
# in order to reduce size of log-files
if hasattr(process, "disableEventDump"):
    process.analyzeZtoMuTauEvents.eventDumps = cms.VPSet()
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# disable accessing generator level information
# if running on data
#from TauAnalysis.Configuration.tools.switchToData import switchToData
#switchToData(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleZtoMuTauSpecific)
#
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
if not hasattr(process, "isBatchMode"):
    process.p.replace(process.producePatTupleZtoMuTauSpecific, process.producePatTuple + process.producePatTupleZtoMuTauSpecific)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
