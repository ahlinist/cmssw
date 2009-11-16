import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runZtoDiTau')

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
# import sequence for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoDiTauSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoDiTau_cff")

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
    maxEventsToPrint = cms.untracked.int32(10)
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
        #'rfio:/castor/cern.ch/user/r/rahmat/hk/rahmat/QCDPt15_GenTrack12_FASTSIM/RAHMAT_QCD/972a8d071c1d3694edbf11929c935ed9/qcdpt15Skim_1.root',
        #'rfio:/castor/cern.ch/user/r/rahmat/hk/rahmat/QCDPt15_GenTrack12_FASTSIM/RAHMAT_QCD/972a8d071c1d3694edbf11929c935ed9/qcdpt15Skim_2.root'
        'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_1.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_2.root'
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

process.p = cms.Path(
    process.producePatTupleZtoDiTauSpecific
# + process.printGenParticleList # uncomment to enable print-out of generator level particles
# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
  + process.selectZtoDiTauEvents 
  + process.analyzeZtoDiTauEvents
  + process.saveZtoDiTauPlots 
)

#--------------------------------------------------------------------------------
#
process.producePatTupleAll = cms.Sequence( process.producePatTuple + process.producePatTupleZtoDiTauSpecific )
#
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction_line01#
#__#patTupleProduction_line02#
if not hasattr(process, "batchMode"):
    process.p.replace(process.producePatTupleZtoDiTauSpecific, process.producePatTuple + process.producePatTupleZtoDiTauSpecific)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# restrict input for PAT-tuple production to AOD event content (instead of RECO/AOD):
#  o use "reducedEcalRecHits" collections for computation of electron isolation variables
#  o disable PAT trigger matching and filling of trigger information histograms
# (necessary to process FastSim samples)
#
# NOTE: the 'switchToAOD' function needs to be called
#       at the very end of configuring the process object !!
#
from TauAnalysis.Configuration.tools.aodTools import *
switchToAOD(process, triggerHistManager = process.triggerHistManagerForDiTau, eventDumpPlugin = process.diTauEventDump)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
