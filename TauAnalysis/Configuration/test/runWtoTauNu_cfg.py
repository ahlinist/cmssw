

import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runWtoTauNu')

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1

#load geometry
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START38_V12::All')

# import particle data table, needed for print-out of generator level information
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#--------------------------------------------------------------------
#import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleWtoTauNuSpecific_cff")

#import sequencefor event selection and running analysis
process.load("TauAnalysis.Configuration.selectWtoTauNu_cff")
process.load("TauAnalysis.Configuration.analyzeWtoTauNu_cff")

# import configuration parameters for submission of jobs to CERN batch system
from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_7TeV_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_10TeV_cfi import *

#--------------------------------------------------------------------------------
# print memory consumed by cmsRun
# (for debugging memory leaks)
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#  ignoreTotal = cms.untracked.int32(1) # default is one
#)
#
#process.printGenParticleList = cms.EDAnalyzer("ParticleListDrawer",
#   src = cms.InputTag("genParticles"),
#   maxEventsToPrint = cms.untracked.int32(100)
#)
#
# print debug information whenever plugins get loaded dynamically from libraries
# (for debugging problems with plugin related dynamic library loading)
#process.add_( cms.Service("PrintLoadingPlugins") )
#---------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.saveWtoTauNuPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsWtoTauNu.root') 
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(1000)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       #'rfio:/castor/cern.ch/user/c/cerati/SkimDataZtautau/tauAnalysisElecMu_skim_TTbar_1_1.root'
        '/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0021/F405BC9A-525D-DF11-AB96-002618943811.root'
       #'rfio:/castor/cern.ch/user/l/liis/wTauNuPatTuples/spring10/shrinkingcone/patTupleWtoTauNu_Wtaunu_7TeV_part01.root'

    )
    #skipBadFiles = cms.untracked.bool(True)
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system (automatically uncommented by scripts)
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.saveWtoTauNuPlots.outputFileName = #plotsOutputFileName#
#__#isBatchMode#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#import utility function for switching pat::Tau input
from PhysicsTools.PatAlgos.tools.tauTools import * 
#switchToCaloTau(process)
switchToPFTauShrinkingCone(process)
#switchToPFTauFixedCone(process)
#switchToPFTauHPSpTaNC(process)

# disable preselection of pat::Taus
# (disabled also in TauAnalysis/RecoTools/python/patPFTauConfig_cfi.py ,
#  but re-enabled after switching tau collection)
process.cleanPatTaus.preselection = cms.string('')
#------------------------------------------------------------------------------
# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *

# uncomment to replace caloJets by pfJets
#switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"))
#-----------------------------------------------------------------------------
# import utility function for managing pat::METs
from TauAnalysis.Configuration.tools.metTools import *

#use raw calo-MET as input
process.patMETs.metSource = cms.InputTag('met')

# uncomment to add pfMET, set Boolean swich to true in order to apply type-1 corrections
addPFMet(process, correct = False)
# replace caloMET by pfMET in all tau-Nu objects
process.load("TauAnalysis.CandidateTools.tauNuPairProduction_cff")
replaceMETforTauNu(process, cms.InputTag('patMETs'),cms.InputTag('patPFMETs'))
#--------------------------------------------------------------------------------
# import utility function for changing cut values
from TauAnalysis.Configuration.tools.changeCut import changeCut
changeCut(process,"selectedPatTausForWTauNuEcalIso","tauID('byIsolation') > 0.5")
changeCut(process,"selectedPatTausForWTauNuTrkIso","tauID('byIsolation') > 0.5")
changeCut(process, "selectedPatElectronsTightId","electronID('eidRobustTight') > 0")
#-----------------------------------------------------------------------------------

#print event content before starting to process 1st event
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")
process.filterFirstEvent = cms.EDFilter("EventCountFilter",
    numEvents = cms.int32(1)
)

process.p = cms.Path( 
    process.producePatTupleWtoTauNuSpecific
#    +process.printGenParticleList # print-out of generator level particles
#    +process.printEventContent    # dump of event content after PAT-tuple production
    +process.selectWtoTauNuEvents
    +process.analyzeWtoTauNuEvents
    +process.saveWtoTauNuPlots 
)


#--------------------------------------------------------------------------------
# import utility function for factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runWtoTauNu
##enableFactorization_runWtoTauNu(process)
#
# define "hook" for enabling/disabling factorization in case running jobs on the CERN batch system 
#(needs to be done after process.p has been defined)
#__#factorization#
#--------------------------------------------------------------------------------
# import utility function for estimation of systematic uncertainties
#from TauAnalysis.Configuration.tools.sysUncertaintyTools import disableSysUncertainties_runWtoTauNu
#
# define "hook" for keeping enabled/disabling estimation of systematic uncertainties
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#systematics#
#if not hasattr(process, "isBatchMode"):
#    disableSysUncertainties_runWtoTauNu(process)

#--------------------------------------------------------------------------------
# disable event-dump output in order to reduce size of log-files
if hasattr(process, "disableEventDump"):
    process.analyzeWtoTauNuEvents.eventDumps = cms.VPSet()
#--------------------------------------------------------------------------------
# disable accessing generator level information if running on data
#from TauAnalysis.Configuration.tools.switchToData import switchToData
#switchToData(process)
#--------------------------------------------------------------------------------

process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleWtoTauNuSpecific)

# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
if not hasattr(process, "isBatchMode"):   
    process.p.replace(process.producePatTupleWtoTauNuSpecific, process.producePatTuple+process.producePatTupleWtoTauNuSpecific)
#--------------------------------------------------------------------------------
# print-out all python configuration parameter information
#print process.dumpPython()
