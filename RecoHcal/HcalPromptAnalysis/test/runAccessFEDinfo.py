import FWCore.ParameterSet.Config as cms

process = cms.Process("AccessFEDinfo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.default.limit = 10
process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
    #'file:myfile.root'
    '/store/data/CRAFT09/AlCaPhiSymHcal/RAW/v1/000/109/933/C22A1CE9-6482-DE11-A245-001D09F24FBA.root'
    #'/store/data/CRAFT09/AlCaPhiSymHcal/RAW/v1/000/109/956/940A333B-6D82-DE11-9760-001D09F276CF.root'
    #'/store/data/CRAFT09/AlCaPhiSymHcal/RAW/v1/000/109/807/9C2A9D86-2C81-DE11-95D3-000423D944DC.root'
    #
    #'/store/data/CRAFT09/TestEnables/RAW/v1/000/109/504/EC8DADBF-DD7D-DE11-A8E9-001D09F24EC0.root',
    #'/store/data/CRAFT09/TestEnables/RAW/v1/000/109/504/DA325A70-DE7D-DE11-AC3D-000423D98E54.root',
    #'/store/data/CRAFT09/TestEnables/RAW/v1/000/109/504/B4088BE3-DF7D-DE11-AB54-000423D98834.root'
    #
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/504/F8CF03B4-DB7D-DE11-B17F-000423D98BE8.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/504/EEEE41B6-DB7D-DE11-AED8-000423D98634.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/504/EE46C1B1-DB7D-DE11-A64D-000423D985B0.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/504/BE79F92F-EB7D-DE11-BF05-001D09F24DDA.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/504/B6E1B7F4-DA7D-DE11-BC38-001D09F23A07.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/504/8C6E6BF2-DA7D-DE11-B359-001D09F2424A.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/504/3082D7B4-DB7D-DE11-B814-000423D94AA8.root'
    #
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/472/D02F49B9-9F7D-DE11-BFDC-001D09F28E80.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/472/BC3C1864-A07D-DE11-9C03-0019B9F581C9.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/472/92D9551C-A17D-DE11-A068-001D09F2423B.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/472/52EE0372-A07D-DE11-A6F9-000423DD2F34.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/472/287957B9-9F7D-DE11-BDFC-001D09F291D2.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/472/186C6AB9-9F7D-DE11-B06E-001D09F25401.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/472/183FA064-A07D-DE11-B4F5-001D09F251D1.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/472/0482AE00-9F7D-DE11-910F-000423D986C4.root',
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/109/472/02AB0C76-B37D-DE11-B0D3-000423D991D4.root'    
    )
)

process.accessFEDinfo = cms.EDAnalyzer('AccessFEDinfo')

process.p = cms.Path(process.accessFEDinfo)
