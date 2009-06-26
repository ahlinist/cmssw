import FWCore.ParameterSet.Config as cms

process = cms.Process("RecoMuon")
# Messages
process.load("FWCore.MessageService.MessageLogger_cfi")


process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/E8D35F0C-03A5-DD11-85BD-003048D15E52.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/D2EA7D04-19A5-DD11-80D6-0030486790C2.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/921235D9-18A5-DD11-B759-003048678B14.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/3A8BEBDE-18A5-DD11-8991-003048D15C44.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/A26169C3-18A5-DD11-89E9-003048678B0E.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/D8E86C02-19A5-DD11-ADB4-003048678FB8.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/7A2D7AF5-18A5-DD11-80FF-003048678A6A.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/48D5C4F8-18A5-DD11-A9B0-003048D15C44.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/C0E01BD8-18A5-DD11-86F7-003048D15DCA.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/C62269FC-18A5-DD11-B2C5-003048678FE0.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/BE14B309-19A5-DD11-A3DB-001A92810A9E.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/62096DE9-18A5-DD11-AC6C-003048767FB3.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/1C82E25F-09A5-DD11-B05B-003048679006.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/3A9443E2-18A5-DD11-B248-0018F3C3E3A6.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/5CF0E4C7-18A5-DD11-8AEB-003048678F8C.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/16CA83E5-18A5-DD11-A709-0018F3D096A0.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/54CA34BF-18A5-DD11-A79B-003048767E4B.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/08C5FDBA-18A5-DD11-9876-003048D15DCA.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/9E4725EE-18A5-DD11-B0F2-0030486790C2.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/D8AE57D9-18A5-DD11-AF66-001A92810A96.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/6E7D93BB-18A5-DD11-B9DE-003048678B94.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/466CEE14-07A5-DD11-AF7C-003048D15F4A.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/9EA232E5-18A5-DD11-95EE-003048678FFE.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/0AE93908-19A5-DD11-A6EA-00304867925E.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/EA3D13BE-18A5-DD11-A936-001A9281170E.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/4A5211C9-18A5-DD11-8F8F-003048D15DBA.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/70AF34FF-18A5-DD11-A6E0-0030486792C8.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/56DEF461-09A5-DD11-9587-003048D15F48.root',
                                                              '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0010/C0BDA80E-19A5-DD11-BC69-0018F3C3E3A6.root'
                                                              )
                            # fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/0005/EEEB64C0-3E37-DE11-84A1-001A92971AA8.root')
                            )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20000)
)

process.TFileService=cms.Service('TFileService',
                                 fileName=cms.string('MyCosmicMuonPlots.root')
                                 )


process.muonAnalysis = cms.EDAnalyzer("CosmicMuonAnalyzer",
                                      MuonCollection = cms.untracked.string('muons'),
                                      )


process.p = cms.Path(process.muonAnalysis)




