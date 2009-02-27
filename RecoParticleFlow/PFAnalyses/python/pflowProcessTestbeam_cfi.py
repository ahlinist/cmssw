import FWCore.ParameterSet.Config as cms

#Define 0T magnetic field
from Configuration.StandardSequences.MagneticField_0T_cff import *
VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

#Change tags for clustering
from RecoParticleFlow.PFClusterProducer.particleFlowCluster_cff import *
particleFlowRecHitECAL.ecalRecHitsEB = cms.InputTag("ecalRecHitMaker", "EcalRecHitsEB")
particleFlowRecHitECAL.ecalRecHitsEE = cms.InputTag("ecalRecHitMaker", "EcalRecHitsEB")
particleFlowRecHitHCAL.hcalRecHitsHBHE = cms.InputTag("hbhereco")
particleFlowRecHitHCAL.caloTowers = cms.InputTag("")
particleFlowRecHitHCAL.isTestbeam = cms.bool(True)

#Change tags for PFBlock building
from RecoParticleFlow.PFBlockProducer.particleFlowBlock_cff import *
particleFlowBlock.RecTracks = cms.InputTag("faketracks", "pfRecTracks")
particleFlowBlock.GsfRecTracks = cms.InputTag("faketracks", "gsfPfRecTracks")
particleFlowBlock.PFClustersPS = cms.InputTag("faketracks", "pfPS")
particleFlowBlock.RecMuons = cms.InputTag("faketracks", "muons")
particleFlowBlock.verbose = cms.untracked.bool(False)
particleFlowBlock.debug = cms.untracked.bool(False)

from Configuration.StandardSequences.Services_cff import *
from Configuration.StandardSequences.Geometry_cff import *
from RecoParticleFlow.PFProducer.particleFlow_cff import *

from Geometry.CaloEventSetup.CaloTowerConstituents_cfi import *
from Configuration.StandardSequences.Reconstruction_cff import *


