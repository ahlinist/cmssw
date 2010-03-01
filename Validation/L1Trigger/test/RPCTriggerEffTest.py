import FWCore.ParameterSet.Config as cms

process = cms.Process("rpctest")

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log')
)



process.SelectRecoMuons = cms.EDFilter("CandSelector",
   filter = cms.bool(True),
   src = cms.InputTag("CloneRecoMuons"),
   cut = cms.string("isGlobalMuon > 0")  # && combinedMuon.normalizedChi2<10 isGlobalMuon > 0
  )


process.CloneRecoMuons = cms.EDProducer("MuonShallowCloneProducer",
     src = cms.InputTag("muons")
)




process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000000)
)

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.test.dqm_onlineEnv_cfi")

process.load("Validation.L1Trigger.RPCTriggerEfficiency_cfi")

process.RPCTriggerVal.MC  = cms.InputTag("SelectRecoMuons")


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

           '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/124/120/F08F782B-77E8-DE11-B1FC-0019B9F72BFF.root'




    )
)


process.dqmSaver.convention = 'Online'
process.dqmSaver.dirName = '.'
process.dqmSaver.producer = 'DQM'
process.dqmEnv.subSystemFolder = 'L1T'

process.a = cms.Path(process.CloneRecoMuons*process.SelectRecoMuons*process.RPCTriggerVal*process.dqmEnv*process.dqmSaver)  
