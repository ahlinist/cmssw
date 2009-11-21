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
#        '/store/relval/CMSSW_3_1_1/RelValWM/GEN-SIM-RECO/STARTUP31X_V1-v2/0002/8E5D0675-E36B-DE11-8F71-001D09F242EF.root'
#'file:/tmp/degrutto/0ABB0814-C082-DE11-9AB7-003048D4767C.root',
#        'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_1_2/RelValZMM/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/AAC0D3F0-CC78-DE11-8E47-001D09F2960F.root'
#'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_1_2/RelValZEE/GEN-SIM-RECO/MC_31X_V3-v1/0007/F0303A91-9278-DE11-AADC-001D09F25456.root'
#'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_1_2/RelValSingleMuPt10/GEN-SIM-RECO/MC_31X_V3-v1/0007/10C5BBC1-9078-DE11-BD31-001D09F231C9.root'
'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_1_2/RelValCosmics/RECO/CRAFT0831X_V1-v1/0007/FE055A0E-A978-DE11-8EF7-001D09F25208.root'
#'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_1_2/RelValWM/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/4899AE78-CB78-DE11-8FC9-000423D98BC4.root'
#         'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_1_2/RelValZMM/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/360B09AE-CC78-DE11-8933-001D09F254CE.root',

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
                     process.ZandWMonitorSelection *
                     process.dqmSaver
                     )

