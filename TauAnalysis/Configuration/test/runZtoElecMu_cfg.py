import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runZtoElecMu')

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
process.GlobalTag.globaltag = cms.string('MC_31X_V2::All')

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoElecMuSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoElecMu_cff")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeZtoElecMu_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecMu_cfi import *
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

process.saveZtoElecMuPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoElecMu.root')
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/relval/CMSSW_3_1_2/RelValZTT/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/A4DD1FAE-B178-DE11-B608-001D09F24EAC.root',
        #'/store/relval/CMSSW_3_1_2/RelValZTT/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/9408B54D-CB78-DE11-9AEB-001D09F2503C.root'
        #'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT31x/muTauSkim_1.root'
        'file:/afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_1_4/src/TauAnalysis/Skimming/test/muTauSkim.root'
    )
    #skipBadFiles = cms.untracked.bool(True)                        
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.analyzeZtoElecMuEvents.filters[0] = copy.deepcopy(#genPhaseSpaceCut#)
#__process.saveZtoElecMuPlots.outputFileName = #plotsOutputFileName#
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
# import utility function for managing pat::METs
from TauAnalysis.Configuration.tools.metTools import *

# uncomment to add pfMET
# set Boolean swich to true in order to apply type-1 corrections
addPFMet(process, correct = False)

# uncomment to replace caloMET by pfMET in all di-tau objects
process.load("TauAnalysis.CandidateTools.diTauPairProductionAllKinds_cff")
replaceMETforDiTaus(process,
                    cms.InputTag('layer1METs'),
                    cms.InputTag('layer1PFMETs'))
#--------------------------------------------------------------------------------

process.p = cms.Path(
    process.producePatTupleZtoElecMuSpecific
# + process.printGenParticleList # uncomment to enable print-out of generator level particles
# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
  + process.selectZtoElecMuEvents 
  + process.analyzeZtoElecMuEvents
  + process.saveZtoElecMuPlots 
)

#--------------------------------------------------------------------------------
# import utility function for factorization
from TauAnalysis.Configuration.factorizationTools import enableFactorization_runZtoElecMu
#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#factorization#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
process.producePatTupleAll = cms.Sequence( process.producePatTuple + process.producePatTupleZtoElecMuSpecific )
#
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
process.p.replace(process.producePatTupleZtoElecMuSpecific, process.producePatTuple + process.producePatTupleZtoElecMuSpecific)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
