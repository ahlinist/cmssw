import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('getSkimPurity')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('MC_31X_V2::All')

process.purityAnalyzer = cms.EDAnalyzer("SkimPurityAnalyzer",
    src = cms.InputTag("genParticles"),
	mode = cms.string('ZtoElecTau')
)

process.printGenParticleList = cms.EDAnalyzer("ParticleListDrawer",
    src = cms.InputTag("genParticles"),
    maxEventsToPrint = cms.untracked.int32(10)
)

process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
		#'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_1.root'
		'file:/data/ndpc2/b/jkolb/eTauSkims/summer09_3_1_4/data/Ztautau_7TeV/skimElecTau_1.root'
    )
)

process.p = cms.Path( process.purityAnalyzer )

#process.options = cms.untracked.PSet(
#	wantSummary = cms.untracked.bool(True)
#)

#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#;print process.dumpPython()
