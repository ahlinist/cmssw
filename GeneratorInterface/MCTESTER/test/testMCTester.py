import FWCore.ParameterSet.Config as cms

process = process = cms.Process("AOD")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('GeneratorInterface.MCTESTER.MCTesterCMS_cfi')
process.maxEvents = cms.untracked.PSet(
#    input = cms.untracked.int32(100)
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:TestHZZ4tau_1.root','file:TestHZZ4tau_2.root','file:TestHZZ4tau_3.root')
                            )

process.p = cms.Path(process.MCTesterCMS+process.endOfProcess)

