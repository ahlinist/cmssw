import FWCore.ParameterSet.Config as cms

process = cms.Process("rpctest")

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log')
)

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.test.dqm_onlineEnv_cfi")

process.load("Validation.L1Trigger.rpcTriggerValidation_cfi")

process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("TrackingTools.RecoGeometry.RecoGeometries_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR_R_36X_V7::All"  

process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')

process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi")
process.load("RecoMuon.DetLayers.muonDetLayerGeometry_cfi")


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_3_5_0_pre1/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V14-v1/0007/88D6E52D-38E8-DE11-B525-003048678BAC.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)



process.rpcTriggerValidation.deltaEtaThreshold=cms.double(0.2)
process.rpcTriggerValidation.deltaPhiThreshold=cms.double(0.25)

process.dqmSaver.convention = 'Online'
process.dqmSaver.dirName = '.'
process.dqmSaver.producer = 'DQM'
process.dqmEnv.subSystemFolder = 'L1T'

process.a = cms.Path(process.rpcTriggerValidation*process.dqmEnv*process.dqmSaver)


  
