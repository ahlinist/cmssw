import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('producePatTupleZtoElecTau')

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_V12::All'

#--------------------------------------------------------------------------------
# import sequence for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")

# define configuration parameters for submission of jobs to CERN batch system 
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecMu_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecMu_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsLooseZtoElecTau_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_cfi import *

# import event-content definition of products to be stored in patTuple
from TauAnalysis.Configuration.patTupleEventContent_cff import *
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# print event content 
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")
#--------------------------------------------------------------------------------

process.savePatTuple = cms.OutputModule("PoolOutputModule",
	patTupleEventContent,
	fileName = cms.untracked.string('patTuple.root')
)

process.maxEvents = cms.untracked.PSet(            
	input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
		'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/PhotonJets_Pt15to20/skimElecTau_PhotonJets_Pt15to20_1.root'
	)
#skipBadFiles = cms.untracked.bool(True)    
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.savePatTuple.fileName = #patTupleOutputFileName#
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

process.p = cms.Path( process.producePatTuple
#                    +process.printEventContent      
			 		 +process.savePatTuple
)

#process.options = cms.untracked.PSet(
#	wantSummary = cms.untracked.bool(True)
#)

# print-out all python configuration parameter information
#print process.dumpPython()
