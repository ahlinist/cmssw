import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runZtoDiTau')

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
process.GlobalTag.globaltag = cms.string('START38_V12::All')

#--------------------------------------------------------------------------------
# import sequence for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoDiTauSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoDiTau_cff")
process.load("TauAnalysis.RecoTools.filterDataQuality_cfi")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeZtoDiTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR; **not implemented for ZtoDiTau channel yet**)
#from TauAnalysis.Configuration.recoSampleDefinitionsZtoDiTau_cfi import *
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

process.saveZtoDiTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoDiTau.root')
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0021/F405BC9A-525D-DF11-AB96-002618943811.root',
        #'/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0020/EE3E8F74-365D-DF11-AE3D-002618FDA211.root'
        'file:/data1/veelken/CMSSW_3_6_x/skims/Ztautau_1_1_sXK.root'
        #'file:/data1/veelken/CMSSW_3_6_x/skims/selEvents_ZtoDiTau_qcdDiJet_RECO.root'
    )    
    #skipBadFiles = cms.untracked.bool(True)
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
# (**not implemented for ZtoDiTau channel yet**)
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.analyzeZtoDiTauEvents.filters[0] = copy.deepcopy(#genPhaseSpaceCut#)
#__process.saveZtoDiTauPlots.outputFileName = #plotsOutputFileName#
#__#isBatchMode#
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
##runBTagging(process, jetCollection = cms.InputTag("ak5CaloJets"), 'AOD')
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

# disable cuts on tau id. discriminators for Track && ECAL isolation (for both taus)
changeCut(process, "selectedPatTausForDiTau1stTrkIso", "tauID('trackIsolation') > -1.")
changeCut(process, "selectedPatTausForDiTau2ndTrkIso", "tauID('trackIsolation') > -1.")
changeCut(process, "selectedPatTausForDiTau2ndTrkIsoLoose", "tauID('trackIsolation') > -1.")
changeCut(process, "selectedPatTausForDiTau1stEcalIso", "tauID('ecalIsolation') > -1.")
changeCut(process, "selectedPatTausForDiTau2ndEcalIso", "tauID('ecalIsolation') > -1.")
changeCut(process, "selectedPatTausForDiTau2ndEcalIsoLoose", "tauID('ecalIsolation') > -1.")

# change cut on TaNC output in case using new HPS + TaNC combined tau id. algorithm
# CV: discriminators by Track/ECAL isolation do not exist for the combined tau id. algorithm
#     and need to be replaced by dummy cuts, in order to avoid run-time exceptions
changeCut(process, "selectedPatTausTrkIso", "tauID('byTaNCtight') > -1.")
changeCut(process, "selectedPatTausForDiTau1stTrkIso", "tauID('byTaNCtight') > -1.")
changeCut(process, "selectedPatTausForDiTau2ndTrkIso", "tauID('byTaNCtight') > -1.")
changeCut(process, "selectedPatTausForDiTau2ndTrkIsoLoose", "tauID('byTaNCtight') > -1.")
changeCut(process, "selectedPatTausEcalIso", "tauID('byTaNCtight') > -1.")
changeCut(process, "selectedPatTausForDiTau1stEcalIso", "tauID('byTaNCtight') > -1.")
changeCut(process, "selectedPatTausForDiTau2ndEcalIso", "tauID('byTaNCtight') > -1.")
changeCut(process, "selectedPatTausForDiTau2ndEcalIsoLoose", "tauID('byTaNCtight') > -1.")
changeCut(process, "selectedPatTausTaNCdiscr", "tauID('byTaNCtight') > 0.5")
changeCut(process, "selectedPatTausForDiTau1stTaNCdiscr", "tauID('byTaNCtight') > 0.5")
changeCut(process, "selectedPatTausForDiTau2ndTaNCdiscr", "tauID('byTaNCtight') > 0.5")
changeCut(process, "selectedPatTausForDiTau2ndTaNCdiscrLoose", "tauID('byTaNCtight') > 0.5")
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------

process.p = cms.Path(
    process.producePatTupleZtoDiTauSpecific
# + process.printGenParticleList # uncomment to enable print-out of generator level particles
# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
  + process.selectZtoDiTauEvents 
  + process.analyzeZtoDiTauEvents
  + process.saveZtoDiTauPlots 
)

process.q = cms.Path(process.dataQualityFilters)

process.schedule = cms.Schedule(process.q, process.p)

#--------------------------------------------------------------------------------
# import utility function for factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runZtoDiTau
#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#factorization#
##enableFactorization_runZtoDiTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for applyting Z-recoil corrections to MET
from TauAnalysis.Configuration.tools.mcToDataCorrectionTools import applyZrecoilCorrection_runZtoDiTau
##applyZrecoilCorrection_runZtoDiTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# disable accessing generator level information
# if running on data
#from TauAnalysis.Configuration.tools.switchToData import switchToData
#switchToData(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
process.producePatTupleAll = cms.Sequence( process.producePatTuple + process.producePatTupleZtoDiTauSpecific )
#
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
if not hasattr(process, "isBatchMode"):
    process.p.replace(process.producePatTupleZtoDiTauSpecific, process.producePatTuple + process.producePatTupleZtoDiTauSpecific)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
