

import FWCore.ParameterSet.Config as cms
import copy
process = cms.Process('runWtoTauNu')
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 500

#load geometry
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('MC_36Y_V7A::All')

# import configuration parameters for submission of jobs to CERN batch system
from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_7TeV_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_10TeV_cfi import *

# Castor FastSimulation
#process.load('FastSimulation.ForwardDetectors.CastorTowerProducer_cfi')
#Castor reconstruction
#process.load('RecoLocalCalo.Castor.Castor_cfi')
#--------------------------------------------------------------------------------
# print memory consumed by cmsRun
# (for debugging memory leaks)
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#  ignoreTotal = cms.untracked.int32(1) # default is one
#)
#---------------------------------------------------------------------------------
process.printGenParticleList = cms.EDAnalyzer("ParticleListDrawer",
   src = cms.InputTag("genParticles"),
   maxEventsToPrint = cms.untracked.int32(100)
)


# import particle data table, needed for print-out of generator level information
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleWtoTauNuSpecific_cff")

process.saveWtoTauNuPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsWtoTauNu.root') 
)

process.load("TauAnalysis.Configuration.selectWtoTauNu_cff")
process.load("TauAnalysis.Configuration.analyzeWtoTauNu_cff")
process.load("TauAnalysis.Configuration.selectWtoTauNu_boosted_cff")
process.load("TauAnalysis.Configuration.analyzeWtoTauNu_boosted_cff")

process.DQMStore = cms.Service("DQMStore")

#print event content
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")

#-------------------------------------------------------------------------------
# print debug information whenever plugins get loaded dynamically from libraries
# (for debugging problems with plugin related dynamic library loading)
#process.add_( cms.Service("PrintLoadingPlugins") )
#--------------------------------------------------------------------------------

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(1000)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_1.root'
        #'rfio:/castor/cern.ch/user/l/liis/CMSSW_31X/SelEvents/MinBias_RawPlusReco_10.root'
    )
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system (automatically uncommented by scripts)
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.saveWtoTauNuPlots.outputFileName = #plotsOutputFileName#
#__#isBatchMode#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for disabling estimation of systematic uncertainties
#
# NOTE: per default, estimation of systematic uncertainties is **enabled** per default
#
from TauAnalysis.Configuration.tools.sysUncertaintyTools import disableSysUncertainties_runWtoTauNu
#
# define "hook" for keeping enabled/disabling estimation of systematic uncertainties
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#systematics#
if not hasattr(process, "isBatchMode"):
    disableSysUncertainties_runWtoTauNu(process)
#--------------------------------------------------------------------------------

from PhysicsTools.PatAlgos.tools.tauTools import * 
switchToPFTauShrinkingCone(process)
#switchToPFTauFixedCone(process)

#--------------------------------------------------------------------------------
# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *

# uncomment to replace caloJets by pfJets
switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"))
#--------------------------------------------------------------------------------

#------------------------Switching to PF-MET and raw Calo-MET----------------------------------
process.patMETs.metSource = cms.InputTag('met')

from TauAnalysis.Configuration.tools.metTools import *

# uncomment to add pfMET, set Boolean swich to true in order to apply type-1 corrections
addPFMet(process, correct = False)
# replace caloMET by pfMET in all tau-Nu objects
process.load("TauAnalysis.CandidateTools.tauNuPairProduction_cff")
replaceMETforTauNu(process,
                                        cms.InputTag('patMETs'),
                                        cms.InputTag('patPFMETs'))

# comment-out to add genMET with mu's to layer1MET (caloMET)
#process.layer1METs.genMETSource = cms.InputTag('genMetTrue')
#--------------------------------------------------------------------------------

from RecoTauTag.RecoTau.PFRecoTauDiscriminationAgainstElectron_cfi import *
pfRecoTauDiscriminationAgainstElectron.ApplyCut_EmFraction = cms.bool(False)
pfRecoTauDiscriminationAgainstElectron.EmFraction_maxValue  = cms.double(0.85)

from TauAnalysis.Configuration.tools.changeCut import *
changeCut(process,"selectedPatTausForWTauNuEcalIso","tauID('byTaNCfrQuarterPercent') > 0.5")
changeCut(process,"selectedPatTausForWTauNuTrkIso","tauID('byTaNCfrQuarterPercent') > 0.5")
changeCut(process, "selectedPatElectronsTightId","electronID('eidRobustTight') > 0")

process.p = cms.Path( 
#    process.CastorTowerReco
#    +process.CastorFullReco
    process.producePatTupleWtoTauNuSpecific
#    +process.printGenParticleList # print-out of generator level particles
#    +process.printEventContent    # dump of event content after PAT-tuple production
    +process.selectWtoTauNuEvents
    +process.analyzeWtoTauNuEvents
#    +process.selectWtoTauNuEventsBoosted
#    +process.analyzeWtoTauNuEventsBoosted
    +process.saveWtoTauNuPlots 
)

#--------------------------------------------------------------------------------
# import utility function for factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runWtoTauNu
#enableFactorization_runWtoTauNu(process)
#from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runWtoTauNu_boosted
#enableFactorization_runWtoTauNu_boosted(process)

#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#factorization#
#--------------------------------------------------------------------------------
process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleWtoTauNuSpecific)

# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
if not hasattr(process, "isBatchMode"):   
    process.p.replace(process.producePatTupleWtoTauNuSpecific, process.producePatTuple+process.producePatTupleWtoTauNuSpecific)
#--------------------------------------------------------------------------------

#replace reco->aod. Necessary for fast-sim produced qcd sample
#from TauAnalysis.Configuration.tools.aodTools import *
#switchToAOD(process, eventDumpPlugin = process.wTauNuEventDump)

# print-out all python configuration parameter information
#print process.dumpPython()
