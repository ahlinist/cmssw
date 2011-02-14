import FWCore.ParameterSet.Config as cms

process = cms.Process("TIMECALIBANALYSIS")

# gfworks: to get clustering 
process.load('Configuration/StandardSequences/GeometryExtended_cff')

# Geometry
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cff")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi") # gfwork: need this? 
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CommonDetUnit.globalTrackingGeometry_cfi") # gfwork: need this?
process.load("Geometry.EcalMapping.EcalMapping_cfi")
process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi") # gfwork: need this?

process.load("Configuration.StandardSequences.MagneticField_38T_cff")

# Global Tag
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff")
#process.GlobalTag.globaltag = 'CRAFT_ALL_V12::All'
process.GlobalTag.globaltag = 'GR_R_35X_V8A::All'

# Trigger
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerScalesConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerPtScaleConfig_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtBoardMapsConfig_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup2_v2_Unprescaled_cff")
import FWCore.Modules.printContent_cfi
process.dumpEv = FWCore.Modules.printContent_cfi.printContent.clone()

import EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi
process.gtDigis = EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi.l1GtUnpack.clone()

# Raw
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
#process.load("RecoLocalCalo.EcalRecProducers.ecalRatioUncalibRecHit_cfi")
process.load("RecoLocalCalo.EcalRecProducers.ecalGlobalUncalibRecHit_cfi")
process.load("RecoLocalCalo.Configuration.ecalLocalRecoSequence_cff")
#process.ecalLocalRecoSequence = cms.Sequence(process.ecalRatioUncalibRecHit*process.ecalDetIdToBeRecovered*process.ecalRecHit+process.ecalPreshowerRecHit)
#process.ecalLocalRecoSequence = cms.Sequence(process.ecalGlobalUncalibRecHit*process.ecalDetIdToBeRecovered*process.ecalRecHit+process.ecalPreshowerRecHit)
#process.ecalRecHit.EEuncalibRecHitCollection = cms.InputTag("ecalGlobalUncalibRecHit","EcalUncalibRecHitsEE")
#process.ecalRecHit.EBuncalibRecHitCollection = cms.InputTag("ecalGlobalUncalibRecHit","EcalUncalibRecHitsEB")
#process.ecalRecHit.EEuncalibRecHitCollection = cms.InputTag("ecalRatioUncalibRecHit","EcalUncalibRecHitsEE")
#process.ecalRecHit.EBuncalibRecHitCollection = cms.InputTag("ecalRatioUncalibRecHit","EcalUncalibRecHitsEB")

process.ecalGlobalUncalibRecHit.doEBtimeCorrection = cms.bool(True)
process.ecalGlobalUncalibRecHit.doEEtimeCorrection = cms.bool(True)

# general basic- and super- clustering sequences
import RecoEcal.EgammaClusterProducers.multi5x5ClusteringSequence_cff

# 3x3 clustering for barrel
process.multi5x5BasicClustersTimePi0Barrel =  RecoEcal.EgammaClusterProducers.multi5x5BasicClusters_cfi.multi5x5BasicClusters.clone(
    # which regions should be clusterized
    doEndcap = cms.bool(False),
    doBarrel = cms.bool(True),

    # gfwork: this is standard, can go away 
    barrelHitProducer = cms.string('ecalRecHit'),
    barrelHitCollection = cms.string('EcalRecHitsEB'),
    endcapHitProducer = cms.string('ecalRecHit'),
    endcapHitCollection = cms.string('EcalRecHitsEE'),
    
    IslandBarrelSeedThr = cms.double(0.5),   # barrelSeedThreshold
    IslandEndcapSeedThr = cms.double(1.0),   # endcapSeedThreshold

    barrelClusterCollection = cms.string('multi5x5BarrelBasicClusters'),
    endcapClusterCollection = cms.string('multi5x5EndcapBasicClusters'),
    clustershapecollectionEE = cms.string('multi5x5EndcapShape'),
    clustershapecollectionEB = cms.string('multi5x5BarrelShape'),
    barrelShapeAssociation = cms.string('multi5x5BarrelShapeAssoc'),
    endcapShapeAssociation = cms.string('multi5x5EndcapShapeAssoc'),
    )


# 3x3 clustering for endcap
process.multi5x5BasicClustersTimePi0Endcap =  RecoEcal.EgammaClusterProducers.multi5x5BasicClusters_cfi.multi5x5BasicClusters.clone(
    # which regions should be clusterized
    doEndcap = cms.bool(True),
    doBarrel = cms.bool(False),

    barrelHitProducer = cms.string('ecalRecHit'),
    barrelHitCollection = cms.string('EcalRecHitsEB'),
    endcapHitProducer = cms.string('ecalRecHit'),
    endcapHitCollection = cms.string('EcalRecHitsEE'),
    
    IslandBarrelSeedThr = cms.double(0.5),              # endcapSeedThreshold
    IslandEndcapSeedThr = cms.double(1.0),             # barrelSeedThreshold

    barrelClusterCollection = cms.string('multi5x5BarrelBasicClusters'),
    endcapClusterCollection = cms.string('multi5x5EndcapBasicClusters'),
    clustershapecollectionEE = cms.string('multi5x5EndcapShape'),
    clustershapecollectionEB = cms.string('multi5x5BarrelShape'),
    barrelShapeAssociation = cms.string('multi5x5BarrelShapeAssoc'),
    endcapShapeAssociation = cms.string('multi5x5EndcapShapeAssoc'),
    )


# super clustering for the ECAL BARREL, staring from multi5x5 3x3 clusters
process.multi5x5SuperClustersTimePi0Barrel =  RecoEcal.EgammaClusterProducers.multi5x5SuperClusters_cfi.multi5x5SuperClusters.clone(
    doBarrel = cms.bool(True),
    doEndcaps = cms.bool(False),
    barrelClusterProducer = cms.string('multi5x5BasicClustersTimePi0Barrel'),
    barrelClusterCollection = cms.string('multi5x5BarrelBasicClusters'),
    endcapClusterProducer = cms.string('multi5x5BasicClustersTimePi0Endcap'),
    endcapClusterCollection = cms.string('multi5x5EndcapBasicClusters')
 )


# super clustering for the ECAL ENDCAP, staring from multi5x5 3x3 clusters
process.multi5x5SuperClustersTimePi0Endcap =  RecoEcal.EgammaClusterProducers.multi5x5SuperClusters_cfi.multi5x5SuperClusters.clone(
    doBarrel = cms.bool(False),
    doEndcaps = cms.bool(True),
    barrelClusterProducer = cms.string('multi5x5BasicClustersTimePi0Barrel'),
    barrelClusterCollection = cms.string('multi5x5BarrelBasicClusters'),
    endcapClusterProducer = cms.string('multi5x5BasicClustersTimePi0Endcap'),
    endcapClusterCollection = cms.string('multi5x5EndcapBasicClusters')
 )




# this is the ntuple producer
process.load("CalibCalorimetry.EcalTiming.ecalTimeTree_cfi")
process.ecalTimeTree.fileName = 'EcalTimeTree'
process.ecalTimeTree.muonCollection = cms.InputTag("muons")
process.ecalTimeTree.runNum = 108645
# gfworks: replathese names
process.ecalTimeTree.barrelSuperClusterCollection = cms.InputTag("multi5x5SuperClustersTimePi0Barrel","multi5x5BarrelSuperClusters")
process.ecalTimeTree.endcapSuperClusterCollection = cms.InputTag("multi5x5SuperClustersTimePi0Endcap","multi5x5EndcapSuperClusters")
process.ecalTimeTree.barrelBasicClusterCollection = cms.InputTag("multi5x5BasicClustersTimePi0Barrel","multi5x5BarrelBasicClusters")
process.ecalTimeTree.endcapBasicClusterCollection = cms.InputTag("multi5x5BasicClustersTimePi0Endcap","multi5x5EndcapBasicClusters")
process.ecalTimeTree.barrelClusterShapeAssociationCollection = cms.InputTag("multi5x5BasicClustersTimePi0Barrel","multi5x5BarrelShapeAssoc")
process.ecalTimeTree.endcapClusterShapeAssociationCollection = cms.InputTag("multi5x5BasicClustersTimePi0Endcap","multi5x5EndcapShapeAssoc") 

process.load("RecoVertex.Configuration.RecoVertex_cff")


process.dumpEvContent = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.p = cms.Path(
    process.ecalDigis *
    process.gctDigis *
    process.gtDigis *
    process.gtEvmDigis *
    process.siPixelDigis *
    process.siStripDigis *
    process.offlineBeamSpot *
    process.trackerlocalreco *
    process.recopixelvertexing *
    process.ckftracks *
    process.ecalPreshowerDigis *
    process.ecalLocalRecoSequence *
    process.multi5x5BasicClustersTimePi0Barrel *
    process.multi5x5BasicClustersTimePi0Endcap *
    process.multi5x5SuperClustersTimePi0Barrel *
    process.multi5x5SuperClustersTimePi0Endcap *
    #process.dumpEvContent  *
    process.vertexreco *
    process.ecalTimeTree
    )



process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    ),
    categories = cms.untracked.vstring('ecalTimeTree'),
    destinations = cms.untracked.vstring('cout')
)




# GF: some legacy reco files to test; replace w/ collision data
# dbs search --query "find file where dataset=/ExpressPhysics/BeamCommissioning09-Express-v2/FEVT and run=124020" | grep store | awk '{printf "\"%s\",\n", $1}'
process.source = cms.Source(
    "PoolSource",
    skipEvents = cms.untracked.uint32(0),

     fileNames = (cms.untracked.vstring(
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/494/A4C5C9FA-C462-DF11-BC35-003048D45F7A.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/480/8CFD761E-C562-DF11-8661-003048D476C4.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/FEE89CEE-F362-DF11-BC8C-003048D476A6.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/FED57876-1B63-DF11-A201-00E0817917F5.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/FEC8FFC6-BF62-DF11-81D3-00E081791787.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/FE52A9F1-ED62-DF11-A2BF-00E0817917EB.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/FE3CA91C-5D61-DF11-B543-003048D476BA.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/FCBED777-AC62-DF11-8234-003048D45FD8.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/FC941F37-0663-DF11-9E2E-003048670B36.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/FC3B55D8-BF62-DF11-8764-0025B3E05CDE.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/FA1F044A-B862-DF11-A17C-001A64789D68.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/F8E0C1BA-C161-DF11-AF90-003048D47736.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/F88FCD4F-9A61-DF11-ADC3-002481E14F7A.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/F8855BDC-BF62-DF11-88EC-002481E15008.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/F8204F63-6A61-DF11-900B-0025B3E063EA.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/F6688BF3-0063-DF11-ABDC-003048D46030.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/F6049B12-AB61-DF11-A6B0-00E0817918D9.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/F483E438-B862-DF11-A20B-003048D45FCA.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/445/F4799A8F-DA60-DF11-8C02-0025B3E06388.root'
                                        )
                  )
    )



