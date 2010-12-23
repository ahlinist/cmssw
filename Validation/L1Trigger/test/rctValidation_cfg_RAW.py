# -*- coding: utf-8 -*-

import sys
import os
#import dbs_discovery
import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTVAL")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("DQMServices.Core.DQM_cfg")

process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("Configuration.EventContent.EventContent_cff")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

#'/store/data/Run2010B/MinimumBias/RAW-RECO/v2/000/146/944/36A3519B-B3CC-DF11-B099-002618943877.root'
#                                '/store/data/Run2010B/EGMonitor/RAW/v1/000/147/826/6A32EF3E-41D6-DF11-966F-001617DBD5AC.root'
       $inputFileNames
#                                '/store/user/swanson/LowECandsRaw/pickevents_156_1_l3O.root'
           #                     '/store/data/Run2010A/EG/RAW/Aug13ReHLTReReco_PreProd_v3/0002/3A6F5B01-F6A9-DF11-A4F9-0030488A0ACE.root'
    ),
#                             noEventSort = cms.untracked.bool(False)
)
#process.options = cms.untracked.PSet (SkipEvent = cms.untracked.vstring('ProductNotFound') )
#process.load("Validation.L1Trigger.L1TRCToffline_133158_cff")
#process.SkipEvent = cms.untracked.PSet (input = cms.untracked.vstring('ProductNotFound'))
#process.load("Validation.L1Trigger.L1TRCToffline_McRelVal_cff")

#process.load("Validation.L1Trigger.L1TRCToffline_McRelVal_cff")
#process.load('L1TriggerConfig.RCTConfigProducers.L1RCTConfig_cff')
 
process.l1RCTParametersTest = cms.EDAnalyzer("L1RCTParametersTester")
process.l1RCTChannelMaskTest = cms.EDAnalyzer("L1RCTChannelMaskTester")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

process.mylocalreco =  cms.Sequence(process.trackerlocalreco*process.calolocalreco)
process.myglobalreco = cms.Sequence(process.offlineBeamSpot+process.recopixelvertexing*process.ckftracks+process.ecalClusters+process.caloTowersRec*process.vertexreco*process.particleFlowCluster)
process.myelectronseeding = cms.Sequence(process.trackerDrivenElectronSeeds*process.ecalDrivenElectronSeeds*process.electronMergedSeeds)
process.myelectrontracking = cms.Sequence(process.electronCkfTrackCandidates*process.electronGsfTracks*process.trackerOnlyConversionSequence)
process.electronFromRaw = cms.Sequence(process.mylocalreco*process.myglobalreco*process.myelectronseeding*process.myelectrontracking*process.particleFlowReco*process.pfElectronTranslator*process.gsfElectronSequence)


#process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")


process.load("Validation.L1Trigger.L1TRCToffline_DataReEmul_cff")
#process.load("Validation.L1Trigger.L1TRCToffline_HE3_cff")

#process.rctheemul = cms.Sequence(process.rctemul+process.rctemul7+process.rctemul6+process.rctemul5+process.rctemul45+process.rctemul4+process.rctemul35+process.rctemul3)
process.highestElectron = cms.EDFilter("LargestPtCandViewSelector",
                           src = cms.InputTag("gsfElectrons"),
                           maxNumber = cms.uint32(1)
                         )
process.selectedPhotonsBarrel = cms.EDFilter("PhotonSelector",
#process.selectedPhotonsBarrel = cms.EDFilter("GsfElectronSelector",
   src = cms.InputTag("photons"),
   cut = cms.string('pt>1.0&&abs(eta)<1.4&&hadronicOverEm()<0.05&&ecalRecHitSumEtConeDR03()<3&&maxEnergyXtal()/e3x3()<0.99&&r2x5>.9'),
   filter = cms.bool(True)
)                                       

process.selectedPhotonsEndcap = cms.EDFilter("PhotonSelector",
   src = cms.InputTag("photons"),
   cut = cms.string('pt>1.0&&abs(eta)>1.4&&hadronicOverEm()<0.05&&ecalRecHitSumEtConeDR03()<3&&maxEnergyXtal()/e3x3()<0.99&&r2x5>.9'),
   filter = cms.bool(True)
)                                       

process.selectedPhotons = cms.EDFilter("PhotonSelector",
   src = cms.InputTag("photons"),
   cut = cms.string('pt>1.0&&hadronicOverEm()<0.05&&ecalRecHitSumEtConeDR03()<3&&maxEnergyXtal()/e3x3()<0.99&&r2x5>.9'),
   filter = cms.bool(True)
)                                       

process.selectedSC = cms.EDFilter("SuperClusterSelector",
   src = cms.InputTag("hybridSuperClusters"),
   cut = cms.string(''),
   filter = cms.bool(True)
)    

process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
   src = cms.InputTag("gsfElectrons"),
   cut = cms.string('pt>1'),
   filter = cms.bool(True)
)    

process.selectedElectronsBarrel = cms.EDFilter("GsfElectronSelector",
   src = cms.InputTag("gsfElectrons"),
   cut = cms.string('pt>1&&isEB()'),
   filter = cms.bool(True)
)    

process.selectedElectronsEndcap = cms.EDFilter("GsfElectronSelector",
   src = cms.InputTag("gsfElectrons"),
   cut = cms.string('pt>1&&isEE()'),
   filter = cms.bool(True)
)    

process.selectedElectronsLow = cms.EDFilter("GsfElectronSelector",
   src = cms.InputTag("gsfElectrons"),
   cut = cms.string('pt>1&&pt<6'),
   filter = cms.bool(True)
)    

process.selectedElectronsTurnOn = cms.EDFilter("GsfElectronSelector",
   src = cms.InputTag("gsfElectrons"),
   cut = cms.string('pt>6&&pt<12'),
   filter = cms.bool(True)
)    

process.selectedElectronsPlateau = cms.EDFilter("GsfElectronSelector",
   src = cms.InputTag("gsfElectrons"),
   cut = cms.string('pt>12&&pt<20'),
   filter = cms.bool(True)
)    

process.selectedElectronsHigh = cms.EDFilter("GsfElectronSelector",
   src = cms.InputTag("gsfElectrons"),
   cut = cms.string('pt>20'),
   filter = cms.bool(True)
)  

process.highestBarrelPhoton = cms.EDFilter("LargestPtCandViewSelector",
                           src = cms.InputTag("selectedPhotonsBarrel"),
                           maxNumber = cms.uint32(1)
                         )



#process.highestEndcapPhoton = cms.EDFilter("LargestPtCandViewSelector",
#                           src = cms.InputTag("selectedPhotonsEndcap"),
#                           maxNumber = cms.uint32(1)
#                         )
#process.highestPhoton = cms.EDFilter("LargestPtCandViewSelector",
#                           src = cms.InputTag("selectedPhotons"),
#                           maxNumber = cms.uint32(1)
#                         )

#process.highestSuperCluster = cms.EDFilter("LargestPtCandViewSelector",
#                           src = cms.InputTag("selectedSC"),
#                           maxNumber = cms.uint32(1)
#                         )

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

process.SC2 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("selectSC"),
                directory = cms.string("L1T/RCTPhotons/SC2"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(2.)              
)

process.gsf2 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                hcalTPGs = cms.InputTag("hcalDigis"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("gsfElectrons"),
                directory = cms.string("L1T/RCTPhotons/GSF2"),
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

process.EGPhot2Emu           = process.EG2.clone()
#process.EGPhot2Emu.ecalTPGs  = cms.InputTag("simEcalTriggerPrimitiveDigis::RCTVAL")
#process.EGPhot2Emu.rctEGamma = cms.InputTag('rctEmulDigis')
process.EGPhot2Emu.directory = cms.string("L1T/RCTPhotonsEmu/EG2Emu")

process.EGPhotB2Emu           = process.EGB2.clone()
#process.EGPhotB2Emu.ecalTPGs  = cms.InputTag("simEcalTriggerPrimitiveDigis::RCTVAL")
#process.EGPhotB2Emu.rctEGamma = cms.InputTag('rctEmulDigis')
process.EGPhotB2Emu.directory = cms.string("L1T/RCTPhotonsEmu/EGB2Emu")
    
process.EGPhotE2Emu           = process.EGE2.clone()
#process.EGPhotE2Emu.ecalTPGs  = cms.InputTag("simEcalTriggerPrimitiveDigis::RCTVAL")
#process.EGPhotE2Emu.rctEGamma = cms.InputTag('rctEmulDigis')
process.EGPhotE2Emu.directory = cms.string("L1T/RCTPhotonsEmu/EGE2Emu")

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

process.EGPhot5Emu           = process.EG5.clone()
#process.EGPhot5Emu.ecalTPGs  = cms.InputTag("simEcalTriggerPrimitiveDigis::RCTVAL")
#process.EGPhot5Emu.rctEGamma = cms.InputTag('rctEmulDigis')
process.EGPhot5Emu.directory = cms.string("L1T/RCTPhotonsEmu/EG5Emu")

process.EGPhotB5Emu           = process.EGB5.clone()
#process.EGPhotB5Emu.ecalTPGs  = cms.InputTag("simEcalTriggerPrimitiveDigis::RCTVAL")
#process.EGPhotB5Emu.rctEGamma = cms.InputTag('rctEmulDigis')
process.EGPhotB5Emu.directory = cms.string("L1T/RCTPhotonsEmu/EGB5Emu")

process.EGPhotE5Emu           = process.EGE5.clone()
#process.EGPhotE5Emu.ecalTPGs  = cms.InputTag("simEcalTriggerPrimitiveDigis::RCTVAL")
#process.EGPhotE5Emu.rctEGamma = cms.InputTag('rctEmulDigis')
process.EGPhotE5Emu.directory = cms.string("L1T/RCTPhotonsEmu/EGE5Emu")

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

process.EGPhot8Emu           = process.EG8.clone()
#process.EGPhot8Emu.ecalTPGs  = cms.InputTag("simEcalTriggerPrimitiveDigis::RCTVAL")
#process.EGPhot8Emu.rctEGamma = cms.InputTag('rctEmulDigis')
process.EGPhot8Emu.directory = cms.string("L1T/RCTPhotonsEmu/EG8Emu")

process.EGPhotB8Emu           = process.EGB8.clone()
#process.EGPhotB8Emu.ecalTPGs  = cms.InputTag("simEcalTriggerPrimitiveDigis::RCTVAL")
#process.EGPhotB8Emu.rctEGamma = cms.InputTag('rctEmulDigis')
process.EGPhotB8Emu.directory = cms.string("L1T/RCTPhotonsEmu/EGB8Emu")
    
process.EGPhotE8Emu           = process.EGE8.clone()
#process.EGPhotE8Emu.ecalTPGs  = cms.InputTag("simEcalTriggerPrimitiveDigis::RCTVAL")
#process.EGPhotE8Emu.rctEGamma = cms.InputTag('rctEmulDigis')
process.EGPhotE8Emu.directory = cms.string("L1T/RCTPhotonsEmu/EGE8Emu")



#Reconfigure Environment and saver
#process.dqmEnv.subSystemFolder = cms.untracked.string('HLT/HLTTAU')
#process.DQM.collectorPort = 9091
#process.DQM.collectorHost = cms.untracked.string('pcwiscms10')
process.DQM.debug = cms.untracked.bool(True)
process.dqmSaver.saveByRun = cms.untracked.int32(-1)
process.dqmSaver.saveAtJobEnd = cms.untracked.bool(True)
process.dqmSaver.workflow = cms.untracked.string('/A/N/C')
process.dqmSaver.forceRunNumber = cms.untracked.int32(555)
process.DQMStore.verbose=0


process.gsf8 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                hcalTPGs = cms.InputTag("hcalDigis"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("selectedElectrons"),
                directory = cms.string("L1T/ALL/GSF8base"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(5.),
                outputFileName = cms.untracked.string('$outputFileName')
#                              outputFileName = cms.untracked.string('outputFileName.root')
)

process.gsfB8          = process.gsf8.clone()
process.gsfB8.genEGamma = cms.InputTag("selectedElectronsBarrel")         
process.gsfB8.directory = cms.string("L1T/BARREL/GSF8base")

process.gsfE8          = process.gsf8.clone()
process.gsfE8.genEGamma = cms.InputTag("selectedElectronsEndcap")         
process.gsfE8.directory = cms.string("L1T/ENDCAP/GSF8base")

process.gsfL8          = process.gsf8.clone()
process.gsfL8.genEGamma = cms.InputTag("selectedElectronsLow")         
process.gsfL8.directory = cms.string("L1T/LOW/GSF8base")

process.gsfTO8          = process.gsf8.clone()
process.gsfTO8.directory = cms.string("L1T/TURNON/GSF8base")
process.gsfTO8.genEGamma = cms.InputTag("selectedElectronsTurnOn")         

process.gsfP8          = process.gsf8.clone()
process.gsfP8.directory = cms.string("L1T/PLATEAU/GSF8base")
process.gsfP8.genEGamma = cms.InputTag("selectedElectronsPlateau")         

process.gsfH8          = process.gsf8.clone()
process.gsfH8.directory = cms.string("L1T/HIGH/GSFbase")
process.gsfH8.genEGamma = cms.InputTag("selectedElectronsHigh")         



process.gsf8iden          = process.gsf8.clone()
process.gsf8iden.directory = cms.string("L1T/ALL/GSF8iden")
process.gsf8iden.rctEGamma = cms.InputTag('rctEmulDigis')

process.gsfB8iden          = process.gsf8iden.clone()
process.gsfB8iden.directory = cms.string("L1T/BARREL/GSF8iden")
process.gsfB8iden.genEGamma = cms.InputTag("selectedElectronsBarrel")         

process.gsfE8iden          = process.gsf8iden.clone()
process.gsfE8iden.directory = cms.string("L1T/ENDCAP/GSF8iden")
process.gsfE8iden.genEGamma = cms.InputTag("selectedElectronsEndcap")         

process.gsfL8iden          = process.gsf8iden.clone()
process.gsfL8iden.directory = cms.string("L1T/LOW/GSF8iden")
process.gsfL8iden.genEGamma = cms.InputTag("selectedElectronsLow")         

process.gsfTO8iden          = process.gsf8iden.clone()
process.gsfTO8iden.directory = cms.string("L1T/TURNON/GSF8iden")
process.gsfTO8iden.genEGamma = cms.InputTag("selectedElectronsTurnOn")

process.gsfP8iden          = process.gsf8iden.clone()
process.gsfP8iden.directory = cms.string("L1T/PLATEAU/GSF8iden")
process.gsfP8iden.genEGamma = cms.InputTag("selectedElectronsPlateau")

process.gsfH8iden          = process.gsf8iden.clone()
process.gsfH8iden.directory = cms.string("L1T/HIGH/GSF8iden")
process.gsfH8iden.genEGamma = cms.InputTag("selectedElectronsHigh")         



process.he01iso15thr12 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                hcalTPGs = cms.InputTag("hcalDigis"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("selectedElectrons"),
                directory = cms.string("L1T/ALL/he01iso15thr12"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(12.),
                outputFileName = cms.untracked.string('$outputFileName'),
                                        iso =cms.untracked.double(1.5),
                                        heCut = cms.untracked.double(.1),
                                        matchL1Objects = cms.untacked.bool(False)

)

process.he005iso15thr12 = process.he10iso15thr12.clone()
process.he005iso15thr12 =  cms.string("L1T/ALL/he005iso15thr12")
process.he005iso15thr12 = heCut = cms.untracked.double(.005)

process.he005iso20thr12 = process.he10iso15thr12.clone()
process.he005iso20thr12.directory =  cms.string("L1T/ALL/he005iso20thr12")
process.he005iso20thr12.heCut = cms.untracked.double(.005)
process.he005iso20thr12.iso = cms.untracked.double(2.0)


process.he005iso25thr12 = process.he10iso15thr12.clone()
process.he005iso25thr12.directory =  cms.string("L1T/ALL/he005iso25thr12")
process.he005iso25thr12.heCut = cms.untracked.double(.005)
process.he005iso25thr12.iso = cms.untracked.double(2.5)


process.he01iso20thr12 = process.he10iso15thr12.clone()
process.he01iso20thr12.directory =  cms.string("L1T/ALL/he01iso20thr12")
process.he01iso20thr12.heCut = cms.untracked.double(.01)
process.he01iso20thr12.iso = cms.untracked.double(2.0)


process.he01iso25thr12 = process.he10iso15thr12.clone()
process.he01iso25thr12.directory =  cms.string("L1T/ALL/he01iso25thr12")
process.he01iso25thr12.heCut = cms.untracked.double(.01)
process.he01iso25thr12.iso = cms.untracked.double(2.5)


process.he01iso15thr20 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                hcalTPGs = cms.InputTag("hcalDigis"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("selectedElectrons"),
                directory = cms.string("L1T/ALL/he01iso15thr12"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(20.),
                outputFileName = cms.untracked.string('$outputFileName'),
                                        iso =cms.untracked.double(1.5),
                                        heCut = cms.untracked.double(.1),
                                        matchL1Objects = cms.untacked.bool(False)

)

process.he005iso15thr20 = process.he10iso15thr12.clone()
process.he005iso15thr20 =  cms.string("L1T/ALL/he005iso15thr20")
process.he005iso15thr20 = heCut = cms.untracked.double(.005)

process.he005iso20thr20 = process.he10iso15thr12.clone()
process.he005iso20thr20.directory =  cms.string("L1T/ALL/he005iso20thr20")
process.he005iso20thr20.heCut = cms.untracked.double(.005)
process.he005iso20thr20.iso = cms.untracked.double(2.0)


process.he005iso25thr20 = process.he10iso15thr12.clone()
process.he005iso25thr20.directory =  cms.string("L1T/ALL/he005iso25thr20")
process.he005iso25thr20.heCut = cms.untracked.double(.005)
process.he005iso25thr20.iso = cms.untracked.double(2.5)


process.he01iso20thr20 = process.he10iso15thr12.clone()
process.he01iso20thr20.directory =  cms.string("L1T/ALL/he01iso20thr20")
process.he01iso20thr20.heCut = cms.untracked.double(.01)
process.he01iso20thr20.iso = cms.untracked.double(2.0)


process.he01iso25thr20 = process.he10iso15thr12.clone()
process.he01iso25thr20.directory =  cms.string("L1T/ALL/he01iso25thr20")
process.he01iso25thr20.heCut = cms.untracked.double(.01)
process.he01iso25thr20.iso = cms.untracked.double(2.5)


process.content = cms.EDAnalyzer("EventContentAnalyzer")

process.MEtoEDMConverter = cms.EDProducer("MEtoEDMConverter",
                                    Name = cms.untracked.string('MEtoEDMConverter'),
                                    Verbosity = cms.untracked.int32(2), # 0 provides no output
                                                                        # 1 provides basic output
                                                                        # 2 provide more detailed output
                                    Frequency = cms.untracked.int32(50),
                                    MEPathToSave = cms.untracked.string('.'),
                                    deleteAfterCopy = cms.untracked.bool(False)
)

process.o = cms.OutputModule("PoolOutputModule",
                             splitLevel = cms.untracked.int32(0),
#                             fileName = cms.untracked.string('$outputFileName'),
                             fileName = cms.untracked.string('outputRawZee.root'),
                                 outputCommands = cms.untracked.vstring(
                                      'drop *_*_*_*',
                                      'keep *_MEtoEDMConverter_*_*'
                                 )

                                )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('GR_R_38X_V13::All')

process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.StandardSequences.DigiToRecoPU")

process.p1 = cms.Path(process.RawToDigi*process.rctemul*#*process.l1RCTParametersTest
                      process.electronFromRaw*
#                      process.gsfElectron*
                      process.selectedElectrons*
                     process.gsf8*
                      process.gsf8iden
)

process.pB = cms.Path(process.RawToDigi*process.rctemul*#*process.l1RCTParametersTest
                      process.electronFromRaw*

                      process.selectedElectronsBarrel*
                     process.gsfB8*
                      process.gsfB8iden
)


process.pE = cms.Path(process.RawToDigi*process.rctemul*#*process.l1RCTParametersTest
                      process.electronFromRaw*
                      process.selectedElectronsEndcap*
                     process.gsfE8*
                      process.gsfE8iden
)

process.pL = cms.Path(process.RawToDigi*process.rctemul*#*process.l1RCTParametersTest
                      process.electronFromRaw*

                      process.selectedElectronsLow*
                     process.gsfL8*
                      process.gsfL8iden
)

process.pTO = cms.Path(process.RawToDigi*process.rctemul*#*process.l1RCTParametersTest
                      process.electronFromRaw*
                      process.selectedElectronsTurnOn*
                     process.gsfTO8*
                      process.gsfTO8iden
)

process.pP = cms.Path(process.RawToDigi*process.rctemul*#*process.l1RCTParametersTest
                      process.electronFromRaw*

                      process.selectedElectronsPlateau*
                     process.gsfP8*
                      process.gsfP8iden
)

process.pH = cms.Path(process.RawToDigi*process.rctemul*#*process.l1RCTParametersTest
                      process.electronFromRaw*
                      process.selectedElectronsHigh*
                     process.gsfH8*
                      process.gsfH8iden
)

process.cutAccess =  cms.Path(process.RawToDigi*process.rctemul*#*process.l1RCTParametersTest
                      process.electronFromRaw*
                              process.he01iso15thr20*
                              process.he005iso15thr20*     
                              process.he005iso20thr20*     
                              process.he005iso25thr20*     
                              process.he01iso20thr20*     
                              process.he01iso25thr20*
                                process.he01iso15thr12*
                              process.he005iso15thr12*     
                              process.he005iso20thr12*     
                              process.he005iso25thr12*     
                              process.he01iso20thr12*     
                              process.he01iso25thr12
                              
                   
)



process.e = cms.EndPath(process.dqmSaver)
#process.Tracer = cms.Service("Tracer")
#process.schedule = cms.Schedule(process.p1,process.pB,process.pE,process.pL,process.pTO,process.pP,process.pH)#,process.e)#,process.p2,process.p3,process.e)
process.schedule = cms.Schedule(process.cutAccess)
#process.schedule = cms.Schedule(process.s1,process.e)
#outfile = open('config.py','w')
#print >> outfile,process.dumpPython()
#outfile.close()
