import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PFAnalyses.pflowExtraction_cfi import *
from RecoParticleFlow.PFAnalyses.pflowFaketracks_cfi import *

from Configuration.StandardSequences.MagneticField_0T_cff import *
VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

TFileService = cms.Service("TFileService",
    fileName=cms.string("outputtree.root")
)

finishup = cms.OutputModule("PoolOutputModule",
    fileName=cms.untracked.string("finishup.root"),
    outputCommands=cms.untracked.vstring('drop *', 'keep recoMuons_*_*_*', 'keep *_faketracks_*_*', 'keep recoPFRecTracks_*_*_*', 'keep recoPFRecHits_*_*_*', 'keep recoPFClusters_*_*_*', 'keep recoPFBlocks_*_*_*', 'keep recoPFCandidates_*_*_*')
    
)

from RecoParticleFlow.PFBlockProducer.particleFlowBlock_cff import *
particleFlowBlock.RecTracks = cms.InputTag("faketracks", "pfRecTracks")
particleFlowBlock.GsfRecTracks = cms.InputTag("faketracks", "gsfPfRecTracks")
particleFlowBlock.PFClustersPS = cms.InputTag("faketracks", "pfPS")
particleFlowBlock.RecMuons = cms.InputTag("faketracks", "muons")
#particleFlowBlock.verbose = cms.untracked.bool(True)
#particleFlowBlock.debug = cms.untracked.bool(True)

from RecoParticleFlow.PFClusterProducer.particleFlowCluster_cff import *
particleFlowRecHitECAL.ecalRecHitsEE = cms.InputTag("ecalRecHitMaker", "EcalRecHitsEB")
particleFlowRecHitECAL.ecalRecHitsEB = cms.InputTag("ecalRecHitMaker", "EcalRecHitsEB")
particleFlowRecHitHCAL.hcalRecHitsHBHE = cms.InputTag("hbhereco")
particleFlowRecHitHCAL.caloTowers = cms.InputTag("")
particleFlowRecHitHCAL.isTestbeam = cms.bool(True)

from Configuration.StandardSequences.Services_cff import *
from Configuration.StandardSequences.Geometry_cff import *
from RecoParticleFlow.PFProducer.particleFlow_cff import *

from Geometry.CaloEventSetup.CaloTowerConstituents_cfi import *
from Configuration.StandardSequences.Reconstruction_cff import *

MessageLogger = cms.Service("MessageLogger",
    debugModules=cms.untracked.vstring('*'),
    cout=cms.untracked.PSet(
        INFO=cms.untracked.PSet(
            limit=cms.untracked.int32(0)
        ),
        PFBlockProducer=cms.untracked.PSet(
            limit=cms.untracked.int32(10000000)
        ),
        PFClusterProducer=cms.untracked.PSet(
            limit=cms.untracked.int32(10000000)
        ),
        DEBUG=cms.untracked.PSet(
            limit=cms.untracked.int32(0)
        ),
        PFProducer=cms.untracked.PSet(
            limit=cms.untracked.int32(10000000)
        ),
        threshold=cms.untracked.string('INFO')
    ),
    categories=cms.untracked.vstring('PFClusterProducer',
        'PFBlockProducer',
        'PFProducer'),
    destinations=cms.untracked.vstring('cout')
)

pflowProcessTestbeam = cms.Sequence(faketracks * 
                                    pfClusteringECAL * 
                                    pfClusteringHCAL * 
                                    particleFlowBlock * 
                                    particleFlow * 
                                    extraction)
