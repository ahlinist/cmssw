import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runZtoElecTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
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
# import sequence for channel-specific production
process.load("TauAnalysis.Configuration.producePatTuple_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoElecTau_cff")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeZtoElecTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
# using skim samples with loose E/p selection and track extra collections
from TauAnalysis.Configuration.recoSampleDefinitionsLooseZtoElecTau_cfi import *

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

process.maxEvents = cms.untracked.PSet(            
		input = cms.untracked.int32(-1)    
		)

process.source = cms.Source("PoolSource",
		fileNames = cms.untracked.vstring(
			'/castor/cern.ch/user/j/jkolb/elecTauAnalysis/patTuples/PhotonJets_Pt15to20/patTupleZtoElecTau_PhotonJets_Pt15to20_1.root"'
			)
		#skipBadFiles = cms.untracked.bool(True)    
		)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.analyzeZtoElecTauEvents.filters[0] = copy.deepcopy(#genPhaseSpaceCut#)
#__process.saveZtoElecTauPlots.outputFileName = #plotsOutputFileName#
#
#--------------------------------------------------------------------------------

process.p = cms.Path( process.produceZtoElecTau
		#					  +process.printEventContent
		+process.selectZtoElecTauEvents 
		+process.analyzeZtoElecTauEvents
		+process.saveZtoElecTauPlots 
		)

#process.options = cms.untracked.PSet(
#	wantSummary = cms.untracked.bool(True)
#)

#--------------------------------------------------------------------------------
# import utility function for factorization
from TauAnalysis.Configuration.factorizationTools import enableFactorization_runZtoElecTau
#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#
#__#factorization#
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
