import FWCore.ParameterSet.Config as cms

process = cms.Process("skimZtoMuTau")

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
process.GlobalTag.globaltag = 'IDEAL_V12::All'

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoMuTauSpecific_cff")
process.load("TauAnalysis.BgEstimationTools.producePatTupleZtoMuTauSpecificForBgEst_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoMuTau_cff")
process.load("TauAnalysis.BgEstimationTools.bgEstPreselZtoMuTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_cfi import *
from TauAnalysis.BgEstimationTools.bgEstSampleDefinitionsZtoMuTau_cfi import *

# import event-content definition of products to be stored in patTuple
from TauAnalysis.Configuration.patTupleEventContent_cff import *
from TauAnalysis.Skimming.EventContent_cff import *
#--------------------------------------------------------------------------------

# print event content 
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/afs/cern.ch/user/v/veelken/scratch0/CMSSW_2_2_10/src/TauAnalysis/Configuration/test/muTauSkim.root'
    )
)

process.saveBgEstSample = cms.OutputModule("PoolOutputModule",
    tauAnalysisEventContent,                                        
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('bgEstSkimPath')
    ),
    fileName = cms.untracked.string('bgEstSample.root')
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.genPhaseSpaceFilter.selector = copy.deepcopy(#genPhaseSpaceCut#)
#__process.saveBgEstSample.fileName = #bgEstSampleOutputFileName#
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

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.bgEstSkimPath = cms.Path(
    process.producePatTuple
   + process.producePatTupleZtoMuTauSpecific
   + process.producePatTupleZtoMuTauSpecificForBgEst
#   + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
   + process.selectZtoMuTauEvents
   + process.genPhaseSpaceFilter
   + process.produceBoolEventSelFlags
   + process.selectEventsByBoolEventSelFlags
)

process.o = cms.EndPath( process.saveBgEstSample )

# print-out all python configuration parameter information
#print process.dumpPython()
