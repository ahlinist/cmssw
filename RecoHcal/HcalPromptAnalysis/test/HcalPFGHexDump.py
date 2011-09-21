import FWCore.ParameterSet.Config as cms

process = cms.Process("DUMP")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20)
)
process.MessageLogger = cms.Service("MessageLogger",
    default = cms.untracked.PSet(
        reportEvery = cms.untracked.int32(1)
    )
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/04610465-56E4-E011-A4EB-003048D3C980.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/04B419EF-59E4-E011-B7AE-003048D2BBF0.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/08F7F85D-56E4-E011-8594-001D09F24047.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/0A2E5B3B-56E4-E011-9F20-BCAEC518FF54.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/107A2D62-56E4-E011-B647-003048CFB40C.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/12CAC1EB-59E4-E011-B189-485B39897227.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/1409C5FB-5AE4-E011-B9B9-003048F11C5C.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/1A9C2845-56E4-E011-BCCD-BCAEC5364C62.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/1C6E6C3C-56E4-E011-9D3D-001D09F2441B.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/1E735146-56E4-E011-9D98-001D09F24D67.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/281F175C-56E4-E011-80DC-BCAEC5329719.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/30915463-56E4-E011-B246-BCAEC53296F8.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/34A1F025-57E4-E011-A10B-E0CB4E55367F.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/34EBB14D-56E4-E011-A277-BCAEC518FF89.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/385AE867-56E4-E011-9B7A-001D09F24489.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/44A1EC56-56E4-E011-A02A-003048D373AE.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/5E1D3CF0-59E4-E011-BAC4-00215AEDFD74.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/62D5BA40-56E4-E011-919A-BCAEC54DB5D6.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/6684AE3F-56E4-E011-81BA-0030487CD76A.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/6CEE5645-56E4-E011-9DDB-003048D374CA.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/72CF41F7-59E4-E011-9B5D-E0CB4E55367F.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/7615994F-56E4-E011-8394-001D09F252DA.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/76B91D4A-56E4-E011-8A91-003048D2BE06.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/78685043-56E4-E011-99B3-003048CFB40C.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/7A4AA64F-56E4-E011-BEC5-001D09F2A465.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/7EB38440-56E4-E011-91F7-0030487CD7EE.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/8243A257-56E4-E011-9923-001D09F26509.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/84A11727-57E4-E011-8553-0030487A17B8.root',
    '/store/express/Run2011B/ExpressPhysics/FEVT/Express-v1/000/176/841/86051056-56E4-E011-AAC4-E0CB4E4408D1.root'
    ),
 #                           lumisToProcess = cms.untracked.VLuminosityBlockRange('176841:60-176841:100'),
                            
#                            eventsToProcess = cms.untracked.VEventRange('123592:755009','123592:392793','123592:79142')
                            
)
#process.dumper = cms.EDAnalyzer("DumpFEDRawDataProduct",
process.dumper = cms.EDAnalyzer("EricDIM",
    DCCversion = cms.untracked.uint32(11298),
    dumpPayload = cms.untracked.bool(False),
    dumpBinary = cms.untracked.bool(True),
    CheckQIEData = cms.untracked.bool(True),
    SuppressQIEOK = cms.untracked.bool(True),
    ForceWriteHistos = cms.untracked.bool(True),
    feds = cms.untracked.vint32(700, 701, 702, 703, 704, 
        705, 706, 707, 708, 709, 
        710, 711, 712, 713, 714, 
        715, 716, 717, 718, 719, 
        720, 721, 722, 723, 724, 
        725, 726, 727, 728, 729, 
        730, 731),
    dumpLevel = cms.untracked.int32(4),
    debug = cms.untracked.bool(False),
#    first_eventnumber = cms.untracked.int32($first_evn),
#    last_eventnumber = cms.untracked.int32($last_evn)
)

process.p = cms.Path(process.dumper)
