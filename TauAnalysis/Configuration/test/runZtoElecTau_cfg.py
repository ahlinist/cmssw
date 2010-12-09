import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runZtoElecTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START38_V14::All')

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoElecTauSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoElecTau_cff")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeZtoElecTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
# using skim samples with loose E/p selection and track extra collections
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_cfi import *

# import event-content definition of products to be stored in patTuple
from TauAnalysis.Configuration.patTupleEventContent_cff import *
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

#--------------------------------------------------------------------------------
# print event content 
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.savePatTuple = cms.OutputModule("PoolOutputModule",
	patTupleEventContent,
	fileName = cms.untracked.string('patTuple.root')
)

process.saveZtoElecTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoElecTau.root')
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
		'/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0021/F405BC9A-525D-DF11-AB96-002618943811.root',
		'/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0020/EE3E8F74-365D-DF11-AE3D-002618FDA211.root'
	)
    #skipBadFiles = cms.untracked.bool(True)    
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.analyzeZtoElecTauEvents.filters[0] = copy.deepcopy(#genPhaseSpaceCut#)
#__process.saveZtoElecTauPlots.outputFileName = #plotsOutputFileName#
#__#isBatchMode#
#__process.GlobalTag.globaltag = "#globalTag#"
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# set trigger

triggerString = cms.vstring('HLT_Ele10_SW_EleId_L1R','HLT_Ele10_LW_EleId_L1R','HLT_Ele10_LW_L1R','HLT_Ele10_SW_L1R','HLT_Ele12_SW_TightEleIdIsol_L1R','HLT_Ele17_SW_TighterEleIdIsol_L1R')
triggerCollection = cms.InputTag('TriggerResults::HLT')

process.Trigger.selectors[0].src = triggerCollection
process.Trigger.selectors[0].triggerPaths = triggerString

process.analyzeZtoElecTauEvents.eventDumps[0].hltPathsToPrint = triggerString
process.analyzeZtoElecTauEvents.eventDumps[0].hltResultsSource = triggerCollection

process.triggerHistManagerForElecTau.hltPaths = triggerString
process.triggerHistManagerForElecTau.hltResultsSource = triggerCollection
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# add patElectronIsolation, which was removed from standard pat sequence in CMSSW_3_4
# only necessary if producing PAT-tuples
#from TauAnalysis.Configuration.tools.patElectronTools import *
#producePatElectronIsolation(process)
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
switchToPFTauShrinkingCone(process)
#switchToPFTauFixedCone(process)

# disable preselection on of pat::Taus
# (disabled also in TauAnalysis/RecoTools/python/patPFTauConfig_cfi.py ,
#  but re-enabled after switching tau collection)
process.cleanPatTaus.preselection = cms.string('')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *

# switchJetCollection complains if this doesn't exist
process.jetCorrFactors = cms.PSet()

# uncomment to replace caloJets by pfJets
switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"))
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
# import utility to remove modules operating on GEN-level collections
from TauAnalysis.Configuration.tools.switchToData import *
#
# uncomment when running over DATA samples
##switchToData(process)#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# for MC, collections are missing
process.analyzeZtoElecTauEvents.eventDumps[0].doGenInfo = cms.bool(False)
#--------------------------------------------------------------------------------

process.p = cms.Path(
    process.producePatTupleZtoElecTauSpecific
# + process.printGenParticleList # uncomment to enable print-out of generator level particles
# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
  + process.selectZtoElecTauEvents 
  + process.analyzeZtoElecTauEvents
  + process.saveZtoElecTauPlots 
)

#process.options = cms.untracked.PSet(
#	wantSummary = cms.untracked.bool(True)
#)

#--------------------------------------------------------------------------------
# import utility function for factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runZtoElecTau
#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#
#__#factorization#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for disabling estimation of systematic uncertainties
#
# NOTE: per default, estimation of systematic uncertainties is **enabled** per default
#
from TauAnalysis.Configuration.tools.sysUncertaintyTools import disableSysUncertainties_runZtoElecTau
#from TauAnalysis.Configuration.tools.sysUncertaintyTools import enableSysUncertainties_runZtoElecTau
#
# define "hook" for keeping enabled/disabling estimation of systematic uncertainties
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#systematics#
if not hasattr(process, "isBatchMode"):
    disableSysUncertainties_runZtoElecTau(process)
    #enableSysUncertainties_runZtoElecTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleZtoElecTauSpecific)
#
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
if not hasattr(process, "isBatchMode"):
    process.p.replace(process.producePatTupleZtoElecTauSpecific, process.producePatTuple + process.producePatTupleZtoElecTauSpecific)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
