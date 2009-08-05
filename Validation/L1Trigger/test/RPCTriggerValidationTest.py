import FWCore.ParameterSet.Config as cms

process = cms.Process("rpctest")

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log')
)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.load("Validation/L1Trigger/RPCTriggerValidation_cfi")

process.RPCTriggerVal.MC  = cms.InputTag("genParticles")
#process.RPCTriggerVal.MC  = cms.InputTag("muons")


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
            '/store/relval/CMSSW_3_1_1/RelValSingleMuPt10/GEN-SIM-RECO/MC_31X_V2-v1/0002/E43E73F7-D56B-DE11-9610-001D09F291D2.root'
    )
)

process.a = cms.Path(process.RPCTriggerVal)

