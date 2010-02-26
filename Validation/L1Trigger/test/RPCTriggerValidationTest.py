import FWCore.ParameterSet.Config as cms

process = cms.Process("rpctest")

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log')
)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000000)
)

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.test.dqm_onlineEnv_cfi")

process.load("Validation.L1Trigger.RPCTriggerValidation_cfi")


#process.RPCTriggerVal.MC  = cms.InputTag("genParticles")
#process.RPCTriggerVal.MC  = cms.InputTag("muons")
#process.RPCTriggerVal.MC  = cms.InputTag("muonsFromCosmics")

#process.RPCTriggerVal.L1CandsCollections = cms.VInputTag(cms.InputTag("gtDigis:CSC"), cms.InputTag("gtDigis:DT") )

#         '/store/relval/CMSSW_3_3_2/RelValSingleMuPt10/GEN-SIM-RECO/MC_31X_V9-v2/0000/D82F1F33-BCC7-DE11-9CF0-003048678F1C.root',
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#           '/store/relval/CMSSW_3_3_2/RelValSingleMuPt10/GEN-SIM-RECO/MC_31X_V9-v2/0000/D82F1F33-BCC7-DE11-9CF0-003048678F1C.root',
           '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/124/120/F08F782B-77E8-DE11-B1FC-0019B9F72BFF.root'
#           '/store/relval/CMSSW_3_3_2/RelValSingleMuPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V9-v2/0000/8E588A2F-BCC7-DE11-A4E0-003048678BF4.root',
# 	    '/store/relval/CMSSW_3_3_2/RelValSingleMuPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V9-v2/0000/8CF89B2F-BCC7-DE11-A1C7-00304867BED8.root',
# 	    '/store/relval/CMSSW_3_3_2/RelValSingleMuPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V9-v2/0000/38717135-BCC7-DE11-80BF-0017319C95D6.root',
# 	    '/store/relval/CMSSW_3_3_2/RelValSingleMuPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V9-v2/0000/28825D2F-BCC7-DE11-9E93-003048678D52.root',
# 	    '/store/relval/CMSSW_3_3_2/RelValSingleMuPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V9-v2/0000/1AD5AF59-BEC7-DE11-BF60-003048678FC6.root',
#	    '/store/relval/CMSSW_3_3_2/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V9-v2/0000/F6CAE95C-BEC7-DE11-B6E6-001731AF6929.root',
# 	    '/store/relval/CMSSW_3_3_2/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V9-v2/0000/2A495EEF-59C8-DE11-8EC8-00248C55CC3C.root',
#	    '/store/relval/CMSSW_3_3_2/RelValSingleMuPt1000/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V9-v2/0000/BE848281-BBC7-DE11-88E3-002618943974.root',
#           '/store/relval/CMSSW_3_3_2/RelValSingleMuPt1000/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V9-v2/0000/A0FCF807-BAC7-DE11-A8F4-001A92971ACC.root',
#           '/store/relval/CMSSW_3_3_2/RelValSingleMuPt1000/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V9-v2/0000/6CF0205D-59C8-DE11-B89A-002618FDA207.root'



    )
)


process.dqmSaver.convention = 'Online'
process.dqmSaver.dirName = '.'
process.dqmSaver.producer = 'DQM'
process.dqmEnv.subSystemFolder = 'L1T'

process.a = cms.Path(process.RPCTriggerVal*process.dqmEnv*process.dqmSaver)


  
