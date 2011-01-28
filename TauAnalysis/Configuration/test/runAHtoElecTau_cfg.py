import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runAHtoElecTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
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

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeAHtoElecTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
# using skim samples with loose E/p selection and track extra collections
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi import *

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

process.saveAHtoElecTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsAHtoElecTau.root')
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
		#'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_1_1_6h9.root'
		'/store/relval/CMSSW_3_8_7/RelValZTT/GEN-SIM-RECO/START38_V13-v1/0016/26155577-92FC-DF11-8E56-001A92810A9A.root',
		'/store/relval/CMSSW_3_8_7/RelValZTT/GEN-SIM-RECO/START38_V13-v1/0016/506F0476-92FC-DF11-8886-00304867C1BC.root',
		'/store/relval/CMSSW_3_8_7/RelValZTT/GEN-SIM-RECO/START38_V13-v1/0017/6262925F-9DFC-DF11-B9EF-0026189438BA.root',
		'/store/relval/CMSSW_3_8_7/RelValZTT/GEN-SIM-RECO/START38_V13-v1/0017/70FD8478-93FC-DF11-90F2-00261894392B.root',
		'/store/relval/CMSSW_3_8_7/RelValZTT/GEN-SIM-RECO/START38_V13-v1/0017/80E7605B-99FC-DF11-AA99-001A9281170E.root',
		'/store/relval/CMSSW_3_8_7/RelValZTT/GEN-SIM-RECO/START38_V13-v1/0017/AA64EF7B-93FC-DF11-AB92-001A92971BC8.root'
	)
    #skipBadFiles = cms.untracked.bool(True)    
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.analyzeAHtoElecTauEvents.filters[0] = copy.deepcopy(#genPhaseSpaceCut#)
#__process.saveAHtoElecTauPlots.outputFileName = #plotsOutputFileName#
#__#isBatchMode#
#__process.GlobalTag.globaltag = "#globalTag#"
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#  make cut changes
from TauAnalysis.Configuration.tools.changeCut import changeCut

#  VBTF WP80 electron ID
changeCut(process,"selectedPatElectronsForElecTauId","(abs(superCluster.eta) < 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.004 & abs(deltaPhiSuperClusterTrackAtVtx) < 0.06 & hcalOverEcal < 0.04 & sigmaIetaIeta < 0.01) | (abs(superCluster.eta) > 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.007 & abs(deltaPhiSuperClusterTrackAtVtx) <0.03 & hcalOverEcal < 0.025 & sigmaIetaIeta < 0.03)")

#  VBTF WP80 electron tracker isolation
#changeCut(process,"selectedPatElectronsForElecTauTrkIso","(abs(superCluster.eta) < 1.479 & dr03TkSumPt/p4.Pt < 0.09) | (abs(superCluster.eta) > 1.479 & dr03TkSumPt/p4.Pt < 0.04)")

#  VBTF WP80 electron ECAL isolation
#changeCut(process,"selectedPatElectronsForElecTauEcalIso","(abs(superCluster.eta) < 1.479 & dr03EcalRecHitSumEt/p4.Pt < 0.04) | (abs(superCluster.eta) > 1.479 & dr03EcalRecHitSumEt/p4.Pt < 0.05)")

#  PF relative iso
#changeCut(process,"selectedPatElectronsForElecTauIso","")

#  remove electron track IP_xy cut
#changeCut(process,"selectedPatElectronsForElecTauTrkIP",cms.double(5),"IpMax")

#  chenge Pt cut for taus
changeCut(process,"selectedPatTausForElecTauPt20","pt > 18")

#  put tau ID at tanc loose
changeCut(process,"selectedPatTausForElecTauTaNCdiscr",'tauID("byTaNCloose") > 0.5')

#  remove 1/3-prong track cut for taus
changeCut(process,"selectedPatTausForElecTauProng","")

#  remove charge = +/-1 cut for taus
changeCut(process,"selectedPatTausForElecTauCharge","")

#  elec/tau overlap cut
changeCut(process,"selectedElecTauPairsAntiOverlapVeto","dR12 > 0.5")

#  transverse mass of electron + MET
changeCut(process,"selectedElecTauPairsMt1MET","mt1MET < 50.")


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

# switchJetCollection complains if this doesn't exist
process.jetCorrFactors = cms.PSet()

# uncomment to replace caloJets by pfJets
switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"),outputModule = "")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for configuring PAT trigger matching
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
switchOnTrigger(process, hltProcess = 'REDIGI38X', outputModule = '')
process.patTrigger.addL1Algos = cms.bool(True)
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
#  do not produce momentum-corrected muons
from TauAnalysis.RecoTools.patLeptonSelection_cff import patMuonSelConfigurator
setattr(patMuonSelConfigurator, "src", "patMuons" )
process.selectPatMuons = patMuonSelConfigurator.configure(process = process)
#--------------------------------------------------------------------------------

process.p = cms.Path(
    process.producePatTupleAHtoElecTauSpecific
# + process.printGenParticleList # uncomment to enable print-out of generator level particles
# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
  + process.selectAHtoElecTauEvents 
  + process.analyzeAHtoElecTauEvents
  + process.saveAHtoElecTauPlots 
)

#process.options = cms.untracked.PSet(
#	wantSummary = cms.untracked.bool(True)
#)

#--------------------------------------------------------------------------------
# import utility function for factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runAHtoElecTau
#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#
#__#factorization#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for applyting Z-recoil corrections to MET
from TauAnalysis.Configuration.tools.mcToDataCorrectionTools import applyZrecoilCorrection_runAHtoElecTau
#applyZrecoilCorrection_runAHtoElecTau(process)
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
	process.analyzeAHtoElecTauEvents.eventDumps = cms.VPSet()
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
