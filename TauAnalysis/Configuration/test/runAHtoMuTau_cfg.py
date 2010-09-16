import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runAHtoMuTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
#process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.FwkReport.reportEvery = 1
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('MC_36Y_V7A::All')

# import particle data table
# needed for print-out of generator level information
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleAHtoMuTauSpecific_cff")

# Options used for SV mass fit method
process.allMuTauPairs.SVOptions.usePtBalanceInFit = cms.bool(True)
process.allMuTauPairs.SVOptions.useMEtInFit = cms.bool(True)
process.allMuTauPairs.SVOptions.useLeg1TrackingInFit = cms.bool(False)
process.allMuTauPairs.SVOptions.useLeg2TrackingInFit = cms.bool(False)
process.allMuTauPairs.SVOptions.correctPrimaryVertexInFit = cms.bool(False)
process.allMuTauPairsLooseMuonIsolation.SVOptions.usePtBalanceInFit = process.allMuTauPairs.SVOptions.usePtBalanceInFit
process.allMuTauPairsLooseMuonIsolation.SVOptions.useMEtInFit = process.allMuTauPairs.SVOptions.useMEtInFit 
process.allMuTauPairsLooseMuonIsolation.SVOptions.useLeg1TrackingInFit = process.allMuTauPairs.SVOptions.useLeg1TrackingInFit
process.allMuTauPairsLooseMuonIsolation.SVOptions.useLeg2TrackingInFit = process.allMuTauPairs.SVOptions.useLeg2TrackingInFit
process.allMuTauPairsLooseMuonIsolation.SVOptions.correctPrimaryVertexInFit = process.allMuTauPairs.SVOptions.correctPrimaryVertexInFit 

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectAHtoMuTau_cff")

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
        #'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_1.root',
        #'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_2.root'
        'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part01.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part02.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part03.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part04.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part05.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part06.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part07.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part08.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part09.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part10.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part11.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part12.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part13.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part14.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part15.root',
        ##'file:/data1/veelken/CMSSW_3_1_2/skims/selEvents_ZtoMuTau_Ztautau_7TeV_part16.root'
    ##),
    ##eventsToProcess = cms.untracked.VEventID(
    ##    '1:386'
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
#
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
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *

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
# import utility function for changing cut values
from TauAnalysis.Configuration.tools.changeCut import changeCut

# change muon track and ECAL isolation requirements
# to relative isolation (sum Pt of tracks/ECAL energy deposits within isolation cone divided by muon Pt)
changeCut(process, "selectedPatMuonsTrkIso", 0.4, attribute = "dRisoCone")
changeCut(process, "selectedPatMuonsTrkIso", 0.09, attribute = "sumPtMax")
changeCut(process, "selectedPatMuonsTrkIso", "relative", attribute = "sumPtMethod")
changeCut(process, "selectedPatMuonsEcalIso", "(ecalIso/pt) < 0.09")

changeCut(process, "selectedPatMuonsTrkIsoLooseIsolation", 0.4, attribute = "dRisoCone")
changeCut(process, "selectedPatMuonsTrkIsoLooseIsolation", 0.25, attribute = "sumPtMax")
changeCut(process, "selectedPatMuonsTrkIsoLooseIsolation", "relative", attribute = "sumPtMethod")
changeCut(process, "selectedPatMuonsEcalIsoLooseIsolation", "(ecalIso/pt) < 0.25")

# enable cut on TaNC output
changeCut(process, "selectedPatTausForMuTauTaNCdiscr", "tauID('byTaNCfrQuarterPercent') > 0.5")
#--------------------------------------------------------------------------------

process.p = cms.Path(
   process.producePatTupleAHtoMuTauSpecific
# + process.printGenParticleList # uncomment to enable print-out of generator level particles
# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
  + process.selectAHtoMuTauEvents
  + process.analyzeAHtoMuTauEvents
  + process.saveAHtoMuTauPlots
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
# import utility function for disabling estimation of systematic uncertainties
#
# NOTE: per default, estimation of systematic uncertainties is **enabled** per default
#
from TauAnalysis.Configuration.tools.sysUncertaintyTools import disableSysUncertainties_runAHtoMuTau
#from TauAnalysis.Configuration.tools.sysUncertaintyTools import enableSysUncertainties_runAHtoMuTau
#
# define "hook" for keeping enabled/disabling estimation of systematic uncertainties
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#systematics#
if not hasattr(process, "isBatchMode"):
    disableSysUncertainties_runAHtoMuTau(process)
    #enableSysUncertainties_runAHtoMuTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# disable event-dump output
# in order to reduce size of log-files
process.disableEventDump = cms.PSet()
if hasattr(process, "disableEventDump"):
    process.analyzeAHtoMuTauEvents_woBtag.eventDumps = cms.VPSet()
    process.analyzeAHtoMuTauEvents_wBtag.eventDumps = cms.VPSet()
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
#
# NOTE: need to delete empty sequence produced by call to "switchJetCollection"
#       in order to avoid error when calling "process.dumpPython"
#      ( cf. https://hypernews.cern.ch/HyperNews/CMS/get/physTools/1688/1/1/1/1/1.html )
#
#del process.patJetMETCorrections
#print process.dumpPython()

#from TauAnalysis.Configuration.tools.switchToData import switchToData
#switchToData(process)

