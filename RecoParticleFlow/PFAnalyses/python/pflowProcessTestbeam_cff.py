import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PFAnalyses.pflowExtraction_cfi import *
from RecoParticleFlow.PFAnalyses.pflowFaketracks_cfi import *
from RecoParticleFlow.PFAnalyses.pflowProcessTestbeam_cfi import *

TFileService = cms.Service("TFileService",
    fileName=cms.string("outputtree.root")
)

finishup = cms.OutputModule("PoolOutputModule",
    fileName=cms.untracked.string("finishup.root"),
    outputCommands=cms.untracked.vstring('drop *', 'keep recoMuons_*_*_*', 'keep *_faketracks_*_*', 'keep recoPFRecTracks_*_*_*', 'keep recoPFRecHits_*_*_*', 'keep recoPFClusters_*_*_*', 'keep recoPFBlocks_*_*_*', 'keep recoPFCandidates_*_*_*')
    
)

#Clustering only
pflowClusteringTestbeam = cms.Sequence(pfClusteringECAL * pfClusteringHCAL)

#Tracking, clustering, pflow - no cleaning
pflowNoCleaning = cms.Sequence(faketracks * pfClusteringECAL * pfClusteringHCAL * particleFlowBlock * particleFlow)

#The works.
pflowProcessTestbeam = cms.Sequence(faketracks * 
                                    pfClusteringECAL * 
                                    pfClusteringHCAL * 
                                    particleFlowBlock * 
                                    particleFlow * 
                                    extraction)
