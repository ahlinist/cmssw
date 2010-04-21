# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTVAL")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("DQMServices.Core.DQM_cfg")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
         $inputFileNames
    ),
    skipBadFiles = cms.untracked.bool(True)                        
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

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

## EG2
process.EG2 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG2"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(2.)              
)

process.EGB2 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestBarrelPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG2_Barrel"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
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
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(2.)              

)                               

## EG5
process.EG5 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG5"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(5.)              
)

process.EGB5 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestBarrelPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG5_Barrel"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
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
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(5.)              

)                               

## EG8
process.EG8 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG8"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(8.)              
)

process.EGB8 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestBarrelPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG8_Barrel"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(8.)              

)                               

process.EGE8 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestEndcapPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG8_Endcap"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(8.)              

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
                     process.EGE5+
                     process.EGE8
)

process.p2 = cms.Path(process.RawToDigi+
                     process.selectedPhotonsBarrel+
                     process.highestBarrelPhoton+
                     process.EGB2+
                     process.EGB5+
                     process.EGB8

)

process.p3 = cms.Path(process.RawToDigi+
                     process.selectedPhotons+
                     process.highestPhoton+
                     process.EG2+
                     process.EG5+
                     process.EG8

)

process.e = cms.EndPath(process.MEtoEDMConverter*process.o)

process.schedule = cms.Schedule(process.p1,process.p2,process.p3,process.e)


