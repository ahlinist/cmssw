

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
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff')
process.GlobalTag.globaltag = 'IDEAL_V9::All'

from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_cfi import * #import data samples

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
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(2000)    
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#    'rfio:/castor/cern.ch/user/l/liis/SelEvents/qcdEvents_passedEcalCrack.root'
    'rfio:/castor/cern.ch/user/l/liis/SelEvents/selEvents_WtoTauNu_Wenu_part01.root',
    'rfio:/castor/cern.ch/user/l/liis/SelEvents/selEvents_WtoTauNu_Wenu_part02.root'
    
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

#------------------------Switching to PF-MET ----------------------------------
from TauAnalysis.Configuration.tools.metTools import *
# comment-out the addPFMet() function to add pfMET: first Boolean switch on production of genMET with mu's, second Boolean switch on type-1 corrections
addPFMet(process,True,False)
# comment-out to replace caloMET by pfMET in all tau-Nu objects
replaceMETforTauNu(process,
                                        cms.InputTag('layer1METs'),
                                        cms.InputTag('layer1PFMETs'))
replaceMETforMet(process,
                 cms.InputTag('layer1METs'),
                 cms.InputTag('layer1PFMETs'))

# comment-out to add genMET with mu's to layer1MET (caloMET)
#process.layer1METs.genMETSource = cms.InputTag('genMETWithMu')
# note: above line works when the first Boolean in the addPFMET is True, otherwise you need comment-out the following:
# addGenMetWithMu(process) # comment-out only when the first Boolean in the addPFMET is False
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.tools.changeCut import *
changeCut(process,"selectedLayer1TausForWTauNuPt20","pt > 20.")
changeCut(process,"selectedLayer1TausForWTauNuTrkIso","tauID('byTaNCfrQuarterPercent') > 0.5")
changeCut(process, "selectedLayer1TausForWTauNuLeadTrkPt","leadPFChargedHadrCand().isNonnull() & leadPFChargedHadrCand().pt() > 20.")
changeCut(process, "selectedLayer1ElectronsTightId","electronID('robust') > 0")


process.p = cms.Path( 
    process.producePatTupleWtoTauNuSpecific
#    +process.printGenParticleList # print-out of generator level particles
#    +process.printEventContent    # dump of event content after PAT-tuple production
    +process.selectWtoTauNuEvents
#    +process.saveWtoTauNuPatTuple # uncomment to write-out produced PAT-tuple
    +process.analyzeWtoTauNuEvents
    +process.saveWtoTauNuPlots 
)

#replace reco->aod. Necessary for fast-sim produced sample
from TauAnalysis.Configuration.tools.aodTools import *
switchToAOD(process)

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

# print-out all python configuration parameter information
#print process.dumpPython()
