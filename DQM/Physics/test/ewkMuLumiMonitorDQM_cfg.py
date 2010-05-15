import FWCore.ParameterSet.Config as cms

process = cms.Process("EwkDQM")
process.load("DQM.Physics.ewkMuLumiMonitorDQM_cfi")


process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")





process.DQM.collectorHost = ''

process.dqmSaver.workflow = cms.untracked.string('/Physics/EWK/LumiMonitor')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(        
"rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_6_1/RelValZMM/GEN-SIM-RECO/START36_V7-v1/0020/566083FA-005D-DF11-B5E5-003048679030.root"
#"rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_6_1/RelValWM/GEN-SIM-RECO/START36_V7-v1/0020/5426D78D-055D-DF11-AE6B-003048678F84.root"
    )
)


process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('detailedInfo'),
    detailedInfo = cms.untracked.PSet(
            default = cms.untracked.PSet( limit = cms.untracked.int32(1) ),
            threshold = cms.untracked.string('INFO')
            #threshold = cms.untracked.string('ERROR'),
     )
)


process.p = cms.Path(
                     process.ewkMuLumiMonitorDQM* 
                     process.dqmSaver
                     )

