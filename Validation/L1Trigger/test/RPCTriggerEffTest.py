import FWCore.ParameterSet.Config as cms

process = cms.Process("rpctest")

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log')
)



process.SelectRecoMuons = cms.EDFilter("CandSelector",
   filter = cms.bool(True),
   src = cms.InputTag("CloneRecoMuons"),
  cut = cms.string("1")# cut = cms.string("isGlobalMuon > 0")  # && combinedMuon.normalizedChi2<10 isGlobalMuon > 0
  )


process.CloneRecoMuons = cms.EDProducer("MuonShallowCloneProducer",
     src = cms.InputTag("muons")
)




process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(200000000)
)

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.test.dqm_onlineEnv_cfi")

process.load("Validation.L1Trigger.RPCTriggerEfficiency_cfi")

process.RPCTriggerVal.MC  = cms.InputTag("SelectRecoMuons")


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

          # '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/124/120/F08F782B-77E8-DE11-B1FC-0019B9F72BFF.root'
           '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/EC38C57D-0507-DF11-82E3-0024E8768CA5.root',
        '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/DC702D75-0507-DF11-8155-0024E8768CCC.root',
        '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/BC4E9469-0507-DF11-BCC0-00151796C1D0.root',
        '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/B8470049-0607-DF11-8FAE-00151796C0DC.root',
        '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/B0AFB674-0507-DF11-BA17-0015178C486C.root',
        '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/AAE4DF40-1307-DF11-80E2-00151796D4F0.root'




    )
)


process.dqmSaver.convention = 'Online'
process.dqmSaver.dirName = '.'
process.dqmSaver.producer = 'DQM'
process.dqmEnv.subSystemFolder = 'L1T'

process.a = cms.Path(process.CloneRecoMuons*process.SelectRecoMuons*process.RPCTriggerVal*process.dqmEnv*process.dqmSaver)  
