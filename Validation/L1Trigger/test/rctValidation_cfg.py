import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTVAL")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("DQMServices.Core.DQM_cfg")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/FC30D37E-AE41-DF11-8897-003048D476E2.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/F63BE431-AE41-DF11-A79D-0025B3E05C6E.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/F4C295F3-B041-DF11-94EF-003048D46060.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/E8EE4DAE-AF41-DF11-BCF8-003048D476A6.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/E23503F5-A941-DF11-8082-001A64789D04.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/DA5DCA8B-A941-DF11-839B-003048670A0C.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/D6C77D9B-AD41-DF11-B636-0025B3E05CF6.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/D4D02E88-A941-DF11-B80A-003048D46098.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/CEDF6A96-AB41-DF11-8A6F-001A6478AB00.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/CA04ECF8-AB41-DF11-BC85-003048D45F72.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/C8C6576F-AF41-DF11-AE9D-0025B3E064AE.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/C85AE5C7-A841-DF11-9C1A-003048D47A6A.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/C4A8463A-AE41-DF11-96DC-003048D45FF8.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/BCF844EE-AE41-DF11-B70D-003048D476A8.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/BAE480D3-A841-DF11-A901-0025B3E05C6E.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/B84CF53C-B041-DF11-A1B8-0025B3E05C6E.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/B69944BF-AC41-DF11-8EA7-0025B31E3C06.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/B020E29A-AD41-DF11-B935-003048D46048.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/AECD7793-AB41-DF11-A265-0025B3E05C6E.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/A4A4C32B-AA41-DF11-AD93-003048D47A44.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/A47B4D38-B041-DF11-855F-00E081791875.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/A43FCDC0-AC41-DF11-B2B0-00E08178C08C.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/A24D845C-AD41-DF11-92DC-00E0817918D3.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/9C7F4FD7-AE41-DF11-BB4B-003048D45FBA.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/9855D9F6-AB41-DF11-9622-003048D46098.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/8C9C3D61-AC41-DF11-BE94-003048D47706.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/8613B020-AA41-DF11-BB84-003048D45F4E.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/849E6E06-AF41-DF11-8281-003048D476F8.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/7C7838E4-AD41-DF11-916F-001A6478AB88.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/7605F379-AE41-DF11-8A7A-00E08178C0B1.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/74C1169D-AD41-DF11-912A-0025B31E3CBE.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/72868ADC-AF41-DF11-ADA9-00E08178C119.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/6ABCEF84-AB41-DF11-8B40-0025B31E3C06.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/662B5D60-AC41-DF11-91B3-003048D46074.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/62771D88-A941-DF11-9113-0025B3E05C6E.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/5EB777CD-AF41-DF11-B3B7-003048D460AC.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/5CBDA9D9-AE41-DF11-B044-0025B3E05DBE.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/569E1A3A-AD41-DF11-BDC0-00E08178C035.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/54504869-AF41-DF11-93DE-003048D476E8.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/503B5B96-AB41-DF11-9461-002481E150FC.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/4AD9E65E-B041-DF11-85E3-0025B3E06584.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/40F898D1-A841-DF11-9676-00E08179178B.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/3EECCB90-AB41-DF11-8428-0025B31E3CBE.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/30E75EC3-AD41-DF11-9396-0025B3E05CF6.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/2EE1EDBA-AC41-DF11-9097-00E081791899.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/1EFC3B2E-AE41-DF11-973C-0025B3E0653E.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/1E49FAA8-AA41-DF11-A895-00E08179183D.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/146AE7C2-AA41-DF11-B9E1-0025B3E05C6E.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/1052ACD9-AE41-DF11-8711-0025B3E05DBE.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/0EF92B4A-B041-DF11-BDCF-003048BAA5A8.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/08CF64D9-A941-DF11-9082-001A6478AB88.root',
               '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/605/0051B6BC-AA41-DF11-91CB-003048673EBA.root',
    ),
    skipBadFiles = cms.untracked.bool(True)                        
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

process.selectedPhotonsBarrel = cms.EDFilter("PhotonSelector",
   src = cms.InputTag("photons"),
   cut = cms.string('pt>1.0&&abs(eta)<1.2&&hadronicOverEm()<0.07&&ecalRecHitSumEtConeDR03()<3&&maxEnergyXtal()/e3x3()<0.99'),
   filter = cms.bool(True)
)                                       

process.selectedPhotonsEndcap = cms.EDFilter("PhotonSelector",
   src = cms.InputTag("photons"),
   cut = cms.string('pt>1.0&&abs(eta)>1.4&&hadronicOverEm()<0.07&&ecalRecHitSumEtConeDR03()<3.&&maxEnergyXtal()/e3x3()<0.99'),
   filter = cms.bool(True)
)                                       

process.selectedPhotons = cms.EDFilter("PhotonSelector",
   src = cms.InputTag("photons"),
   cut = cms.string('pt>1.0&&hadronicOverEm()<0.07&&ecalRecHitSumEtConeDR03()<3.&&maxEnergyXtal()/e3x3()<0.99'),
   filter = cms.bool(True)
)                                       

process.highestBarrelPhoton = cms.EDProducer("LargestPtCandViewSelector",
                           src = cms.InputTag("selectedPhotonsBarrel"),
                           maxNumber = cms.uint32(1)
                         )
process.highestEndcapPhoton = cms.EDProducer("LargestPtCandViewSelector",
                           src = cms.InputTag("selectedPhotonsEndcap"),
                           maxNumber = cms.uint32(1)
                         )
process.highestPhoton = cms.EDProducer("LargestPtCandViewSelector",
                           src = cms.InputTag("selectedPhotons"),
                           maxNumber = cms.uint32(1)
                         )


process.EGB2 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestBarrelPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG2_Barrel"),
                maxEt = cms.untracked.double(20),
                binsEt = cms.untracked.int32(20),
                gammaThreshold = cms.untracked.double(2.)              

)                               

process.EGE2 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestEndcapPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG2_Endcap"),
                maxEt = cms.untracked.double(20),
                binsEt = cms.untracked.int32(20),
                gammaThreshold = cms.untracked.double(2.)              

)                               

process.EGB5 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestBarrelPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG5_Barrel"),
                maxEt = cms.untracked.double(20),
                binsEt = cms.untracked.int32(20),
                gammaThreshold = cms.untracked.double(5.)              

)                               

process.EGE5 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestEndcapPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG5_Endcap"),
                maxEt = cms.untracked.double(20),
                binsEt = cms.untracked.int32(20),
                gammaThreshold = cms.untracked.double(5.)              

)                               

process.EG2 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG2"),
                maxEt = cms.untracked.double(20),
                binsEt = cms.untracked.int32(20),
                gammaThreshold = cms.untracked.double(2.)              
)

process.EG5 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG5"),
                maxEt = cms.untracked.double(20),
                binsEt = cms.untracked.int32(20),
                gammaThreshold = cms.untracked.double(5.)              
)
#Reconfigure Environment and saver
#process.dqmEnv.subSystemFolder = cms.untracked.string('HLT/HLTTAU')
#process.DQM.collectorPort = 9091
#process.DQM.collectorHost = cms.untracked.string('pcwiscms10')

process.dqmSaver.saveByRun = cms.untracked.int32(-1)
process.dqmSaver.saveAtJobEnd = cms.untracked.bool(True)
process.dqmSaver.workflow = cms.untracked.string('/A/N/C')
process.dqmSaver.forceRunNumber = cms.untracked.int32(123)


process.MEtoEDMConverter = cms.EDFilter("MEtoEDMConverter",
                                    Name = cms.untracked.string('MEtoEDMConverter'),
                                    Verbosity = cms.untracked.int32(0), # 0 provides no output
                                                                        # 1 provides basic output
                                                                        # 2 provide more detailed output
                                    Frequency = cms.untracked.int32(50),
                                    MEPathToSave = cms.untracked.string(''),
                                    deleteAfterCopy = cms.untracked.bool(False)
)

process.o = cms.OutputModule("PoolOutputModule",
                             splitLevel = cms.untracked.int32(0),
                             fileName = cms.untracked.string('$outputFileName'),
                                 outputCommands = cms.untracked.vstring(
                                      'drop *_*_*_*',
                                      'keep *_MEtoEDMConverter_*_*'
                                 )

                                )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('GR10_P_V3::All')

process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Geometry_cff")


process.p1 = cms.Path(process.RawToDigi+
                     process.selectedPhotonsEndcap+
                     process.highestEndcapPhoton+
                     process.EGE2+
                     process.EGE5
)

process.p2 = cms.Path(process.RawToDigi+
                     process.selectedPhotonsBarrel+
                     process.highestBarrelPhoton+
                     process.EGB2+
                     process.EGB5

)

process.p3 = cms.Path(process.RawToDigi+
                     process.selectedPhotons+
                     process.highestPhoton+
                     process.EG2+
                     process.EG5

)

process.e = cms.EndPath(process.MEtoEDMConverter*process.o)

process.schedule = cms.Schedule(process.p1,process.p2,process.p3,process.e)


