

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
process.GlobalTag.globaltag = cms.string('MC_31X_V2::All')

# import configuration parameters for submission of jobs to CERN batch system
from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_cfi import *

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

process.DQMStore = cms.Service("DQMStore")

#print event content
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")

#-------------------------------------------------------------------------------
# print debug information whenever plugins get loaded dynamically from libraries
# (for debugging problems with plugin related dynamic library loading)
#process.add_( cms.Service("PrintLoadingPlugins") )
#--------------------------------------------------------------------------------

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#    'rfio:/castor/cern.ch/user/l/liis/SelEvents/qcdEvents_passedEcalCrack.root'
   'rfio:/castor/cern.ch/user/l/liis/SelEvents/selEvents_WtoTauNu_Wtaunu_part01.root'
#    'rfio:/castor/cern.ch/user/l/liis/SelEvents/selEvents_WtoTauNu_Wenu_part02.root'    
#    'rfio:/castor/cern.ch/user/l/liis/SelEvents/qcdEvents_passedEcalCrack.root'
#    'file:QCDSkim_1.root'   
#    'file:QCD_PtTrack15_FASTSIM_1000.root'
#    'file:/afs/cern.ch/user/l/liis/scratch0/CMSSW_2_2_13/src/TauAnalysis/Skimming/test/QCDSkim.root'
    )
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system (automatically uncommented by scripts)
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.saveWtoTauNuPlots.outputFileName = #plotsOutputFileName#
#--------------------------------------------------------------------------------


from PhysicsTools.PatAlgos.tools.tauTools import * 
switchToPFTauShrinkingCone(process)
#switchToPFTauFixedCone(process)

#------------------------Switching to PF-MET and raw Calo-MET----------------------------------
process.layer1METs.metSource = cms.InputTag('met')

from TauAnalysis.Configuration.tools.metTools import *

# comment-out the addPFMet() function to add pfMET: first Boolean switch on production of genMET with mu's, second Boolean switch on type-1 corrections
addPFMet(process,True,False)
# replace caloMET by pfMET in all tau-Nu objects
process.load("TauAnalysis.CandidateTools.tauNuPairProduction_cff")
replaceMETforTauNu(process,
                                        cms.InputTag('layer1METs'),
                                        cms.InputTag('layer1PFMETs'))

# comment-out to add genMET with mu's to layer1MET (caloMET)
#process.layer1METs.genMETSource = cms.InputTag('genMetTrue')
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.tools.changeCut import *
changeCut(process,"selectedLayer1TausForWTauNuPt20","pt > 20.")
changeCut(process,"selectedLayer1TausForWTauNuTrkIso","tauID('byTaNCfrQuarterPercent') > 0.5")
changeCut(process, "selectedLayer1TausForWTauNuLeadTrkPt","leadPFChargedHadrCand().isNonnull() & leadPFChargedHadrCand().pt() > 20.")
changeCut(process, "selectedLayer1ElectronsTightId","electronID('robust') > 0")


process.p = cms.Path( 
    process.producePatTupleWtoTauNuSpecific
#    +process.layer1CaloMETs
#    +process.printGenParticleList # print-out of generator level particles
#    +process.printEventContent    # dump of event content after PAT-tuple production
    +process.selectWtoTauNuEvents
    +process.analyzeWtoTauNuEvents
    +process.saveWtoTauNuPlots 
)

#--------------------------------------------------------------------------------
# import utility function for factorization
#from TauAnalysis.Configuration.factorizationTools import enableFactorization_runWtoTauNu
#enableFactorization_runWtoTauNu(process)

#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#factorization#
#--------------------------------------------------------------------------------
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
process.p.replace(process.producePatTupleWtoTauNuSpecific, process.producePatTuple+process.producePatTupleWtoTauNuSpecific)
#--------------------------------------------------------------------------------

#replace reco->aod. Necessary for fast-sim produced qcd sample
from TauAnalysis.Configuration.tools.aodTools import *
switchToAOD(process)

# print-out all python configuration parameter information
#print process.dumpPython()
