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
    input = cms.untracked.int32(100)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
        '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0012/5A79C5DB-AA45-DF11-B1B8-00215E93F06C.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/F2952E05-6F44-DF11-A267-E41F13181D5C.root',
##         '/store/mc/Spring10/PhotonJet_Pt30/AODSIM/START3X_V26_S09-v1/0042/FACF2723-2549-DF11-AE0B-0017A4770C10.root',
##         '/store/mc/Spring10/PhotonJet_Pt30/AODSIM/START3X_V26_S09-v1/0042/F81945F9-2549-DF11-BC15-001E0B472CEE.root',
##         '/store/mc/Spring10/PhotonJet_Pt30/AODSIM/START3X_V26_S09-v1/0042/F00DE021-2649-DF11-92BF-002264984988.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/F26705DA-6E44-DF11-9EAC-00215E93E7AC.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/D8514CF6-7244-DF11-8AE7-00215E21DFBA.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/D4D0EE40-6F44-DF11-9739-E41F13181B60.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/D2D518F7-7244-DF11-9A1D-E41F131815BC.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/D0C387E5-7444-DF11-BC80-E41F13181460.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/D085E799-7144-DF11-966F-00215E93E7AC.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/C6219DF9-7444-DF11-B4AB-00215E93EF9C.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/C2A5C27B-7544-DF11-8549-E41F13181688.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/BECE1389-7444-DF11-A07A-00215E93E7AC.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/BE8863A1-7044-DF11-A324-00215E222772.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/BC6DAE25-7344-DF11-BF14-00215E21D57C.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/B86A056A-7044-DF11-9437-00215E93C8B0.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/B6EFDCA0-7044-DF11-8542-00215E22201C.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/B00B27D1-7144-DF11-9F96-00215E2211F4.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/A6FC85DF-6D44-DF11-8CC1-E41F1318157C.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/A41A98B1-6D44-DF11-B30E-E41F1318158C.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/A0FE9789-7444-DF11-A606-00215E2220F4.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/9C1FFB05-6E44-DF11-9FDE-00215E93D738.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/929CAA8C-7444-DF11-AA34-00215E93C4A8.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/88507C83-7544-DF11-9C2B-E41F131816A8.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/86E4199F-7044-DF11-980C-00215E222256.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/82F7C0D3-7144-DF11-9F8A-00215E21D750.root',
##         '/store/mc/Spring10/PhotonJet_Pt15/AODSIM/START3X_V26_S09-v1/0005/82DCB2DD-6D44-DF11-BDA2-E41F131817E8.root'
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

