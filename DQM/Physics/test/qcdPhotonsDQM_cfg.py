import FWCore.ParameterSet.Config as cms

process = cms.Process("QcdPhotonsDQM")
process.load("DQM.Physics.qcdPhotonsDQM_cfi")

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.DQM.collectorHost = ''

process.dqmSaver.workflow = cms.untracked.string('/My/Test/DataSet')

## Geometry and Detector Conditions (needed for spike removal code)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START3X_V20::All')
process.load("Configuration.StandardSequences.MagneticField_cff")


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20000)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0012/5A79C5DB-AA45-DF11-B1B8-00215E93F06C.root',
    '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/F2952E05-6F44-DF11-A267-E41F13181D5C.root',
    '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/F26705DA-6E44-DF11-9EAC-00215E93E7AC.root',
    '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/D8514CF6-7244-DF11-8AE7-00215E21DFBA.root',
    '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/D4D0EE40-6F44-DF11-9739-E41F13181B60.root'
#        '/store/data/Run2010A/EG/RECO/v1/000/136/086/165A1B30-4767-DF11-AFBD-0030487C7392.root',
#        '/store/data/Run2010A/EG/RECO/v1/000/136/084/8472E9F2-4367-DF11-B309-000423D98750.root',
#        '/store/data/Run2010A/EG/RECO/v1/000/136/083/14F5EC18-2667-DF11-857A-000423D8F63C.root',
#        '/store/data/Run2010A/EG/RECO/v1/000/136/082/38099728-E466-DF11-ABC5-001D09F244BB.root',
#        '/store/data/Run2010A/EG/RECO/v1/000/136/082/301FF51A-FE66-DF11-84A9-001D09F29114.root',
#        '/store/data/Run2010A/EG/RECO/v1/000/136/082/18825C67-DC66-DF11-9AA7-000423D6CA6E.root',
#        '/store/data/Run2010A/EG/RECO/v1/000/136/082/14677069-EA66-DF11-BDFA-000423D98DC4.root',
#        '/store/data/Run2010A/EG/RECO/v1/000/136/080/B4FA9047-C766-DF11-AC4A-001D09F28E80.root'
                           )
)

process.p = cms.Path(process.qcdPhotonsDQM+process.dqmSaver)

