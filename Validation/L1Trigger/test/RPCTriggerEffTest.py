import FWCore.ParameterSet.Config as cms

process = cms.Process("rpctest")

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log')
)

process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
  vertexCollection = cms.InputTag('offlinePrimaryVertices'),
  minimumNDOF = cms.uint32(4) ,
  maxAbsZ = cms.double(15),	
  maxd0 = cms.double(2)	
)

process.SelectRecoMuons = cms.EDFilter("CandSelector",
   #filter = cms.bool(True),
   #filter = cms.bool(False),
   src = cms.InputTag("CloneRecoMuons"),
  cut = cms.string("isGlobalMuon > 0 && pt > 2")  # && combinedMuon.normalizedChi2<10 isGlobalMuon > 0 isMuon > 0
  )


process.CloneRecoMuons = cms.EDProducer("MuonShallowCloneProducer",
     src = cms.InputTag("muons")
)



#process.out = cms.OutputModule("PoolOutputModule",
    #fileName = cms.untracked.string('Out.root'),
    #outputCommands = cms.untracked.vstring(
		        
                        #"drop *"
			
     #)
#)




process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)





process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load("Geometry.MuonCommonData.muonIdealGeometryXML_cfi")
#process.load("Geometry.RPCGeometry.rpcGeometry_cfi")
#process.load("Geometry.CSCGeometry.cscGeometry_cfi")
#process.load("Geometry.DTGeometry.dtGeometry_cfi")
#process.load("Geometry.CommonDetUnit.globalTrackingGeometry_cfi")
process.load("TrackingTools.RecoGeometry.RecoGeometries_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR10_P_V5::All"  

process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')

process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi")
process.load("RecoMuon.DetLayers.muonDetLayerGeometry_cfi")

#Real data raw to digi
#process.load("Configuration.StandardSequences.RawToDigi_Data_cff")






process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.test.dqm_onlineEnv_cfi")


from RecoMuon.TrackingTools.MuonServiceProxy_cff import * 
from RecoMuon.TrackingTools.MuonTrackLoader_cff import *


process.load("Validation.L1Trigger.RPCTriggerEfficiency_cfi")

#process.RPCTriggerVal.MCCollection  = cms.InputTag("muons")
process.RPCTriggerVal.MCCollection  = cms.InputTag("SelectRecoMuons")
process.RPCTriggerVal.deltaEtaThreshold=cms.double(0.2)
process.RPCTriggerVal.deltaPhiThreshold=cms.double(0.25)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
	
 
 '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/601/F85204EE-EB40-DF11-8F71-001A64789D1C.root'

    )
)


process.dqmSaver.convention = 'Online'
process.dqmSaver.dirName = '.'
process.dqmSaver.producer = 'DQM'
process.dqmEnv.subSystemFolder = 'L1T'

#process.a =cms.Path(process.RPCTriggerVal*process.dqmEnv*process.dqmSaver) process.CloneRecoMuons*process.SelectRecoMuons*process.RawToDigi*
process.a=cms.Path(process.primaryVertexFilter*process.CloneRecoMuons*process.SelectRecoMuons*process.RPCTriggerVal*process.dqmEnv*process.dqmSaver)  




