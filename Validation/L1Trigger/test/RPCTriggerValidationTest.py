import FWCore.ParameterSet.Config as cms

process = cms.Process("rpctest")

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log')
)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.test.dqm_onlineEnv_cfi")

process.load("Validation/L1Trigger/RPCTriggerValidation_cfi")

process.RPCTriggerVal.MC  = cms.InputTag("genParticles")
#process.RPCTriggerVal.MC  = cms.InputTag("muons")

#process.RPCTriggerVal.L1CandsCollections = cms.VInputTag(cms.InputTag("gtDigis:CSC"), cms.InputTag("gtDigis:DT") )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
            '/store/relval/CMSSW_3_3_2/RelValSingleMuPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V9-v2/0000/8E588A2F-BCC7-DE11-A4E0-003048678BF4.root'
    )
)

process.dqmSaver.convention = 'Online'
process.dqmSaver.dirName = '.'
process.dqmSaver.producer = 'DQM'
process.dqmEnv.subSystemFolder = 'L1T'

process.a = cms.Path(process.RPCTriggerVal*process.dqmEnv*process.dqmSaver)


  
