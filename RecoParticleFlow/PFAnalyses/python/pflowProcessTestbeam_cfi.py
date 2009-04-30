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
particleFlowBlock.PFClustersHFEM = cms.InputTag("faketracks", "pfHFHAD")
particleFlowBlock.PFClustersHFHAD = cms.InputTag("faketracks", "pfHFEM")
particleFlowBlock.RecMuons = cms.InputTag("faketracks", "muons")
particleFlowBlock.verbose = cms.untracked.bool(False)
particleFlowBlock.debug = cms.untracked.bool(False)


particleFlowBlock.pf_DPtoverPt_Cut = cms.vdouble(100.0,1.0,0.80,0.50)
particleFlowBlock.pf_NHit_Cut = cms.vuint32(0,3,3,6)

particleFlowRecHitHCAL.thresh_Barrel = cms.double(0.0)
particleFlowClusterHCAL.thresh_Seed_Barrel = cms.double(1.4)
particleFlowClusterHCAL.thresh_Barrel = cms.double(0.8)
particleFlowBlock.pf_chi2_ECAL_HCAL = cms.double(100.0)

from Configuration.StandardSequences.Services_cff import *
from Configuration.StandardSequences.Geometry_cff import *
from RecoParticleFlow.PFProducer.particleFlow_cff import *

from Geometry.CaloEventSetup.CaloTowerConstituents_cfi import *
from Configuration.StandardSequences.Reconstruction_cff import *


