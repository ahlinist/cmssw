import FWCore.ParameterSet.Config as cms


from RecoParticleFlow.PFAnalyses.pflowCalibratable_cfi import *
from RecoParticleFlow.PFAnalyses.pflowFaketracks_cfi import *
from RecoParticleFlow.PFAnalyses.pflowProcessTestbeam_cfi import *
from RecoParticleFlow.PFAnalyses.pflowParticleFiltration_cfi import *

TFileService = cms.Service("TFileService",
    fileName=cms.string("outputtree.root")
)

finishup = cms.OutputModule("PoolOutputModule",
    fileName=cms.untracked.string("reprocessed.root"),
    #outputCommands=cms.untracked.vstring('keep *'),
    outputCommands=cms.untracked.vstring('drop *', 'keep *_particleFiltration_*_*', 'keep recoMuons_*_*_*', 'keep *_calibratable_*_*', 'keep *_faketracks_*_*', 'keep recoPFRecTracks_*_*_*', 'keep recoPFRecHits_*_*_*', 'keep recoPFClusters_*_*_*', 'keep recoPFBlocks_*_*_*', 'keep recoPFCandidates_*_*_*'),
	SelectEvents=cms.untracked.PSet(
                SelectEvents=cms.vstring('p1')
    ) 
)

#particleFlow.pf_newCalib = cms.uint32(2)
particleFlow.pfcluster_globalP0 = cms.double(1.54)                              
particleFlow.pfcluster_globalP1 = cms.double(0.90)
particleFlow.pfcluster_lowEP0 = cms.double(-1.98)
particleFlow.pfcluster_lowEP1 = cms.double(1.11)
particleFlow.ecalHcalEcalBarrel = cms.vdouble(0.67, 3.0, 1.15, 0.90, - 0.035, 1.7)
particleFlow.ecalHcalEcalEndcap = cms.vdouble(0.11, 10.0, 1.10, 0.40, - 0.00, 1.7)

#Clustering only
pflowClusteringTestbeam = cms.Sequence(pfClusteringECAL * pfClusteringHCAL)

#Tracking, clustering, pflow - no cleaning
pflowNoCleaning = cms.Sequence(faketracks * pfClusteringECAL * pfClusteringHCAL * particleFlowBlock * particleFlow)

#The works.
pflowProcessTestbeam = cms.Sequence(particleFiltration * faketracks * 
                                    pfClusteringECAL * 
                                    pfClusteringHCAL * 
                                    particleFlowBlock * 
                                    particleFlow * extraction)
