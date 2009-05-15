import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runZtoElecMu')

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
process.load("TauAnalysis.Configuration.selectZtoElecMu_cff")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeZtoElecMu_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecMu_cfi import *

# import event-content definition of products to be stored in patTuple
from TauAnalysis.Configuration.patTupleEventContent_cff import *
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.saveZtoElecMuPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoElecMu.root')
)

process.saveZtoElecMuPatTuple = cms.OutputModule("PoolOutputModule",
    patTupleEventContent,                                                   
    fileName = cms.untracked.string('elecMuSkim_patTuple.root')
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#
# Z --> tau tau (all decay modes; simulated with TAUOLA)
# 10k events RelVal sample
#
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/elecMuSkim.root'
#        'file:/afs/cern.ch/user/v/veelken/scratch0/CMSSW_2_2_7/src/TauAnalysis/Configuration/test/muTauSkim.root'    
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
#---This_is_a_Hook_for_Replacement_of_outputFileName_Parameter_of_DQMSimpleFileSaver
#
# to be replaced by e.g.
#  "process.saveZtoElecMuPlots.outputFileName = plotsOutputFileNameQCD_BCtoE_Pt20to30"
#
#---This_is_a_Hook_for_Replacement_of_fileName_Parameter_of_PoolOutputModule
#
# to be replaced by e.g.
#  "process.saveZtoElecMuPatTuple.fileName = patTupleOutputFileNameQCD_BCtoE_Pt20to30"
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
# import utility function for managing pat::METs
from TauAnalysis.Configuration.tools.metTools import *
# comment-out to add pfMET
# first Boolean swich on genMET with mu's production
# second Boolean swich on type-1 corrections
addPFMet(process,True,False)
# comment-out to replce caloMET by pfMET in all di-tau objects
replaceMETforDiTaus(process,
                    cms.InputTag('layer1METs'),
                    cms.InputTag('layer1PFMETs'))
#--------------------------------------------------------------------------------

process.p = cms.Path( process.producePatTuple
#                    +process.content               # uncomment to enable dump of event content after PAT-tuple production
                     +process.selectZtoElecMuEvents
#                    +process.saveZtoElecMuPatTuple # uncomment to write-out produced PAT-tuple  
                     +process.analyzeZtoElecMuEvents
                     +process.saveZtoElecMuPlots )

# print-out all python configuration parameter information
#print process.dumpPython()
