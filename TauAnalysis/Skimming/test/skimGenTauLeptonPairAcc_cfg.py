import FWCore.ParameterSet.Config as cms

process = cms.Process("genTauLeptonsPairAccSkim")

from TauAnalysis.Skimming.EventContent_cff import *

process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('MC_31X_V2::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_3_1_2/RelValZTT/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/A4DD1FAE-B178-DE11-B608-001D09F24EAC.root',
        '/store/relval/CMSSW_3_1_2/RelValZTT/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/9408B54D-CB78-DE11-9AEB-001D09F2503C.root'
    )
)

#--------------------------------------------------------------------------------
# select generated tau+/tau- leptons
#--------------------------------------------------------------------------------

process.selectedGenTauPlusLeptons = cms.EDFilter("GenParticleSelector",
    src = cms.InputTag('genParticles'),
    cut = cms.string("pdgId == -15 && pt > 12 & abs(eta) < 2.5"),
    filter = cms.bool(True)
)

process.selectedGenTauMinusLeptons = cms.EDFilter("GenParticleSelector",
    src = cms.InputTag('genParticles'),
    cut = cms.string("pdgId == +15 && pt > 12 & abs(eta) < 2.5"),
    filter = cms.bool(True)
)

#--------------------------------------------------------------------------------
# keep event in case generated tau+ as well as generated tau- are within acceptance
#--------------------------------------------------------------------------------

process.genTauLeptonsPairAccSkimPath = cms.Path(
   process.selectedGenTauPlusLeptons * process.selectedGenTauMinusLeptons
)

genTauLeptonsPairAccEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('genTauLeptonsPairAccSkimPath')
    )
)

process.genTauLeptonsPairAccSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    tauAnalysisEventContent,                                               
    genTauLeptonsPairAccEventSelection,
    fileName = cms.untracked.string('genTauLeptonsPairAccSkim.root')
)


process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.genTauLeptonsPairAccSkimOutputModule)

