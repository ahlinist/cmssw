import FWCore.ParameterSet.Config as cms

process = cms.Process("TIMECALIBANALYSIS")

# gfworks: to get clustering 
process.load('Configuration/StandardSequences/GeometryExtended_cff')

# Geometry
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cff")
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.EcalMapping.EcalMapping_cfi")
process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi") # gfwork: need this?


# Global Tag
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff")
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
process.load("CalibCalorimetry.EcalTiming.ecalTimeEleTree_cfi")
process.ecalTimeEleTree.fileName = 'EcalTimeTree'
process.ecalTimeEleTree.muonCollection = cms.InputTag("muons")
process.ecalTimeEleTree.runNum = 108645
# gfworks: replathese names
process.ecalTimeEleTree.barrelSuperClusterCollection = cms.InputTag("multi5x5SuperClustersTimePi0Barrel","multi5x5BarrelSuperClusters")
process.ecalTimeEleTree.endcapSuperClusterCollection = cms.InputTag("multi5x5SuperClustersTimePi0Endcap","multi5x5EndcapSuperClusters")
process.ecalTimeEleTree.barrelBasicClusterCollection = cms.InputTag("multi5x5BasicClustersTimePi0Barrel","multi5x5BarrelBasicClusters")
process.ecalTimeEleTree.endcapBasicClusterCollection = cms.InputTag("multi5x5BasicClustersTimePi0Endcap","multi5x5EndcapBasicClusters")
process.ecalTimeEleTree.barrelClusterShapeAssociationCollection = cms.InputTag("multi5x5BasicClustersTimePi0Barrel","multi5x5BarrelShapeAssoc")
process.ecalTimeEleTree.endcapClusterShapeAssociationCollection = cms.InputTag("multi5x5BasicClustersTimePi0Endcap","multi5x5EndcapShapeAssoc") 






#
#################################################################################################
#################################################################################################
####    P r e p a r a t i o n      o f    t h e    P A T    O b j e c t s   f r o m    A O D  ###
#################################################################################################
#
### pat sequences to be loaded:
#process.load("PhysicsTools.PatAlgos.patSequences_cff")
#
##
### %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
### MET creation     <=== WARNING: YOU MAY WANT TO MODIFY THIS PART OF THE CODE       %%%%%%%%%%%%%
###                                specify the names of the MET collections that you need here %%%%
###                                                                                             #%%
### if you don't specify anything the default MET is the raw Calo MET                           #%%
#process.caloMET = process.patMETs.clone(                                                       #%%
#            metSource = cms.InputTag("met","","RECO"),
#                        addTrigMatch = cms.bool(False),
#                        addMuonCorrections = cms.bool(False),
#                        addGenMET = cms.bool(False),
#                    )
#process.tcMET = process.patMETs.clone(                                                         #%%
#            metSource = cms.InputTag("tcMet","","RECO"),
#                        addTrigMatch = cms.bool(False),
#                        addMuonCorrections = cms.bool(False),
#                        addGenMET = cms.bool(False),
#                    )
#process.pfMET = process.patMETs.clone(                                                         #%%
#            metSource = cms.InputTag("pfMet","","RECO"),
#                        addTrigMatch = cms.bool(False),
#                        addMuonCorrections = cms.bool(False),
#                        addGenMET = cms.bool(False),
#                    )
### specify here what you want to have on the plots! <===== MET THAT YOU WANT ON THE PLOTS  %%%%%%%
#myMetCollection   = 'caloMET'
#myPfMetCollection =   'pfMET'
#myTcMetCollection =   'tcMET'
#
### specify here what you want to have on the plots! <===== MET THAT YOU WANT ON THE PLOTS  %%%%%%%
### myDesiredMetCollection = 'layer1RawCaloMETs'
### modify the sequence of the MET creation:                                                    #%%
#process.makePatMETs = cms.Sequence(process.caloMET*process.tcMET*process.pfMET)
#
# ## GF changed here: more than one pat done here
### %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
### modify the final pat sequence: keep only electrons + METS (muons are needed for met corrections)
#process.load("RecoEgamma.EgammaIsolationAlgos.egammaIsolationSequence_cff")
##process.patElectronIsolation = cms.Sequence(process.egammaIsolationSequence)
#
#process.patElectrons.isoDeposits = cms.PSet()
#process.patElectrons.userIsolation = cms.PSet()
#process.patElectrons.addElectronID = cms.bool(True)
#process.patElectrons.electronIDSources = cms.PSet(
#        simpleEleId95relIso= cms.InputTag("simpleEleId95relIso"),
#            simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
#            simpleEleId85relIso= cms.InputTag("simpleEleId85relIso"),
#            simpleEleId80relIso= cms.InputTag("simpleEleId80relIso"),
#            simpleEleId70relIso= cms.InputTag("simpleEleId70relIso"),
#            simpleEleId60relIso= cms.InputTag("simpleEleId60relIso"),
#            simpleEleId95cIso= cms.InputTag("simpleEleId95cIso"),
#            simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
#            simpleEleId85cIso= cms.InputTag("simpleEleId85cIso"),
#            simpleEleId80cIso= cms.InputTag("simpleEleId80cIso"),
#            simpleEleId70cIso= cms.InputTag("simpleEleId70cIso"),
#            simpleEleId60cIso= cms.InputTag("simpleEleId60cIso"),
#            )
###
#process.patElectrons.addGenMatch = cms.bool(False)
#process.patElectrons.embedGenMatch = cms.bool(False)
#process.patElectrons.usePV = cms.bool(False)
###
##process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
##  difference between data and MC
## JM ==> set to TRUE to tell the ID that it is data
## JM ==> or set to talse if it's MC
#process.simpleEleId95relIso.dataMagneticFieldSetUp = cms.bool(True)
#process.simpleEleId90relIso.dataMagneticFieldSetUp = cms.bool(True)
#process.simpleEleId85relIso.dataMagneticFieldSetUp = cms.bool(True)
#process.simpleEleId80relIso.dataMagneticFieldSetUp = cms.bool(True)
#process.simpleEleId70relIso.dataMagneticFieldSetUp = cms.bool(True)
#process.simpleEleId60relIso.dataMagneticFieldSetUp = cms.bool(True)
#process.simpleEleId95cIso.dataMagneticFieldSetUp = cms.bool(True)
#process.simpleEleId90cIso.dataMagneticFieldSetUp = cms.bool(True)
#process.simpleEleId85cIso.dataMagneticFieldSetUp = cms.bool(True)
#process.simpleEleId80cIso.dataMagneticFieldSetUp = cms.bool(True)
#process.simpleEleId70cIso.dataMagneticFieldSetUp = cms.bool(True)
#process.simpleEleId60cIso.dataMagneticFieldSetUp = cms.bool(True)
#
#process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
##process.makePatElectrons = cms.Sequence(process.patElectronIDs*process.patElectrons)
## process.makePatMuons may be needed depending on how you calculate the MET
##process.makePatCandidates = cms.Sequence(process.makePatElectrons+process.makePatMETs)
##process.patDefaultSequence = cms.Sequence(process.makePatCandidates)
#









process.dumpEvContent = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.p = cms.Path(
    #process.patDefaultSequence *
    process.multi5x5BasicClustersTimePi0Barrel *
    process.multi5x5BasicClustersTimePi0Endcap *
    process.multi5x5SuperClustersTimePi0Barrel *
    process.multi5x5SuperClustersTimePi0Endcap *
    # process.dumpEvContent  *
    process.ecalTimeEleTree
    )



process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    ),
    categories = cms.untracked.vstring('ecalTimeEleTree'),
    destinations = cms.untracked.vstring('cout')
)




# GF: some legacy reco files to test; replace w/ collision data
# dbs search --query "find file where dataset=/ExpressPhysics/BeamCommissioning09-Express-v2/FEVT and run=124020" | grep store | awk '{printf "\"%s\",\n", $1}'
process.source = cms.Source(
    "PoolSource",
    skipEvents = cms.untracked.uint32(0),

    # a few files from:    /MinimumBias/Commissioning10-GR_R_35X_V7A_SD_EG-v2/RECO
    fileNames = (cms.untracked.vstring(
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/494/A4C5C9FA-C462-DF11-BC35-003048D45F7A.root',
    #'/store/relval/CMSSW_4_2_0_pre8/EG/RECO/GR_R_42_V7_RelVal_wzEG2010A-v1/0043/069662C9-9A56-E011-9741-0018F3D096D2.root'
    #'/store/data/Run2010A/EG/RECO/v4/000/144/114/EEC21BFA-25B4-DF11-840A-001617DBD5AC.root'

    'file:/data/franzoni/data/CMSSW_4_2_0_pre8__EG__RECO__GR_R_42_V7_RelVal_wzEG2010A-v1__0043__069662C9-9A56-E011-9741-0018F3D096D2.root'
     )
                  )
    )



