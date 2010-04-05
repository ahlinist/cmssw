import FWCore.ParameterSet.Config as cms

process = cms.Process("rpctest")

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log')
)



process.SelectRecoMuons = cms.EDFilter("CandSelector",
   #filter = cms.bool(True),
   #filter = cms.bool(False),
   src = cms.InputTag("CloneRecoMuons"),
  cut = cms.string("1.0")  # && combinedMuon.normalizedChi2<10 isGlobalMuon > 0 isMuon > 0
  )


process.CloneRecoMuons = cms.EDProducer("MuonShallowCloneProducer",
     src = cms.InputTag("muons")
)



process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('Out.root'),
    outputCommands = cms.untracked.vstring(
		        
                        "drop *"
			
     )
)




process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(200000000)
)

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.test.dqm_onlineEnv_cfi")

process.load("Validation.L1Trigger.RPCTriggerEfficiency_cfi")

process.RPCTriggerVal.MCCollection  = cms.InputTag("SelectRecoMuons")
process.RPCTriggerVal.deltaRThreshold=cms.double(2.5)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
	
           #'/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/124/120/F08F782B-77E8-DE11-B1FC-0019B9F72BFF.root'
           #'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/EC38C57D-0507-DF11-82E3-0024E8768CA5.root',
        #'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/DC702D75-0507-DF11-8155-0024E8768CCC.root',
        #'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/BC4E9469-0507-DF11-BCC0-00151796C1D0.root',
        #'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/B8470049-0607-DF11-8FAE-00151796C0DC.root',
        #'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/B0AFB674-0507-DF11-BA17-0015178C486C.root',
        #'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/AAE4DF40-1307-DF11-80E2-00151796D4F0.root'
	#'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/F4C92A98-163C-DF11-9788-0030487C7392.root'
	#"file:/afs/cern.ch/user/d/dbart/scratch0/CMSSW_3_6_0_pre4/dbart/run132440_198_199.root",
		 
	#"file:/afs/cern.ch/user/d/dbart/scratch0/CMSSW_3_6_0_pre4/dbart/run132440_192_197.root"
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440001.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440002.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440003.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440004.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440005.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440006.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440007.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440008.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440009.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440010.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440011.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440012.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440013.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440014.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440015.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440016.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440017.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440018.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440019.root",
	"/store/caf/user/stupputi/Beams/MBTrigger/Skims/MuSkimSkimmed_132440020.root"
	#'rfio:/castor/cern.ch/user/d/dbart/Out.root'
	#'/store/data/Commissioning10/MinimumBias/RAW-RECO/v7/000/132/476/2AB80D2C-613D-DF11-B785-00E08178C039.root',
        #'/store/data/Commissioning10/MinimumBias/RAW-RECO/v7/000/132/476/0ECE0114-5E3D-DF11-AE1F-0025B3E05D8C.root'
    )
)


process.dqmSaver.convention = 'Online'
process.dqmSaver.dirName = '.'
process.dqmSaver.producer = 'DQM'
process.dqmEnv.subSystemFolder = 'L1T'

#process.a =cms.Path(process.RPCTriggerVal*process.dqmEnv*process.dqmSaver)
process.a=cms.Path(process.CloneRecoMuons*process.SelectRecoMuons*(process.RPCTriggerVal*process.dqmEnv*process.dqmSaver+process.out))  




