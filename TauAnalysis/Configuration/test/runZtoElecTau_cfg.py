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
#process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff')
process.GlobalTag.globaltag = 'IDEAL_V9::All'

#--------------------------------------------------------------------------------
# import sequence for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoElecTau_cff")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeZtoElecTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_cfi import *

# import event-content definition of products to be stored in patTuple
from TauAnalysis.Configuration.patTupleEventContent_cff import *
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# print event content 
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.saveZtoElecTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoElecTau.root')
)

process.saveZtoElecTauPatTuple = cms.OutputModule("PoolOutputModule",
    patTupleEventContent,
    #fileName = cms.untracked.string('/data/jkolb/elecTauAnalysis/elecTauSkim_patTuple.root')
    fileName = cms.untracked.string('elecTauSkim_patTuple.root')
)

# control frequency of message logger print-out
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(600)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_Ztautau_1.root'
        'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_1.root'
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
#   extEventSelection.insert(process.analyzeZtoElecTau.eventSelection)
#   process.analyzeZtoElecTau.eventSelection = extEventSelection"
#
#---This_is_a_Hook_for_Replacement_of_outputFileName_Parameter_of_DQMSimpleFileSaver
#
# to be replaced by e.g.
#  "process.saveZtoElecTauPlots.outputFileName = plotsOutputFileNameQCD_BCtoE_Pt20to30"
#
#---This_is_a_Hook_for_Replacement_of_fileName_Parameter_of_PoolOutputModule
#
# to be replaced by e.g.
#  "process.saveZtoElecTauPatTuple.fileName = patTupleOutputFileNameQCD_BCtoE_Pt20to30"
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
#switchToPFTauShrinkingCone(process)
switchToPFTauFixedCone(process)
#--------------------------------------------------------------------------------
process.p = cms.Path( process.producePatTuple
#                    +process.printEventContent      # uncomment to enable dump of event content after PAT-tuple production
                     +process.selectZtoElecTauEvents
#                    +process.saveZtoElecTauPatTuple # uncomment to write-out produced PAT-tuple                      
                     +process.analyzeZtoElecTauEvents
                     +process.saveZtoElecTauPlots )

# print-out all python configuration parameter information
#print process.dumpPython()
