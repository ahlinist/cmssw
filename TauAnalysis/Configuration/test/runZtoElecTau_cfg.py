import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runZtoElecTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.load("TauAnalysis.Configuration.producePatLayer1_cff")
process.load("TauAnalysis.RecoTools.pftauPatSelectorForElecTau_cfi")
process.load("TauAnalysis.CandidateTools.elecTauPairProducer_cff")

process.load("TauAnalysis.Configuration.analyzeZtoElecTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.sampleDefinitionsZtoElecTau_cfi import *

process.DQMStore = cms.Service("DQMStore")

process.saveZtoElecTau = cms.EDAnalyzer("DQMSimpleFileSaver",
  outputFileName = cms.string('plotsZtoElecTau.root')
)

# control output frequency
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')

#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#  ignoreTotal = cms.untracked.int32(1) # default is one
#)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(1)    
)

process.source = cms.Source("PoolSource",
    #firstEvent = cms.untracked.uint32(4097),
    #firstRun = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_1.root'
#        'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_1.root'
    )
    #skipBadFiles = cms.untracked.bool(True)    
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system:
#
#---This_is_a_Hook_for_Replacement_of_fileNames_Parameter
#
# to be replaced by e.g.
#
#  "process.source.fileNames = fileNamesQCD_BCtoE_Pt20to30"
#
#---This_is_a_Hook_for_Replacement_of_maxEvents_Parameter
#
# to be replaced by e.g.
#
#  "process.maxEvents.input = cms.untracked.int32(100)"
#
#---This_is_a_Hook_for_Replacement_of_genPhaseSpaceCut_Parameter
#
# to be replaced by e.g.
#
#  "extEventSelection = cms.VPSet()
#   extEventSelection.insert(genPhaseSpaceCutQCD_BCtoE_Pt20to30)
#   extEventSelection.insert(process.analyzeZtoElecMu.eventSelection)
#   process.analyzeZtoElecMu.eventSelection = extEventSelection"
#
#---This_is_a_Hook_for_Replacement_of_outputFileName_Parameter
#
# to be replaced by e.g.
#  "process.saveZtoElecMu.outputFileName = outputFileNameQCD_BCtoE_Pt20to30"
#
#--------------------------------------------------------------------------------

process.p = cms.Path( process.producePatLayer1ForTauAnalyses
                     +process.selectPFTausForElecTau
                     +process.produceElecTauPairs
                     +process.analyzeZtoElecTau
                     +process.saveZtoElecTau ) 

# print-out all python configuration parameter information
#print process.dumpPython()
