import FWCore.ParameterSet.Config as cms


import RecoParticleFlow.PFAnalyses.pflowCalibratable_cfi as calibratable 
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

#Clustering only
pflowClusteringTestbeam = cms.Sequence(pfClusteringECAL * pfClusteringHCAL)

#Tracking, clustering, pflow - no cleaning
pflowNoCleaning = cms.Sequence(faketracks * pfClusteringECAL * pfClusteringHCAL * particleFlowBlock * particleFlow)

extraction = cms.EDAnalyzer("ExtractionAnalyzer",
    calibratable.TestbeamDelegate
)

#The works.
pflowProcessTestbeam = cms.Sequence(particleFiltration * faketracks * 
                                    pfClusteringECAL * 
                                    pfClusteringHCAL * 
                                    particleFlowBlock * 
                                    particleFlow *
                                    extraction )
