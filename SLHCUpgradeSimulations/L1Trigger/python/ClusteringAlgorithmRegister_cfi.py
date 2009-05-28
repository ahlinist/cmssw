import FWCore.ParameterSet.Config as cms

ClusteringAlgorithm_a_PSimHit_ = cms.ESProducer("ClusteringAlgorithm_a_PSimHit_")

ClusteringAlgorithm_a_PixelDigi_ = cms.ESProducer("ClusteringAlgorithm_a_PixelDigi_")

ClusteringAlgorithm_a_TTHit_ = cms.ESProducer("ClusteringAlgorithm_a_TTHit_")


"""ClusteringAlgorithm_broadside_PSimHit_ = cms.ESProducer("ClusteringAlgorithm_broadside_PSimHit_" , WidthCut = cms.int32(3) )  # WidthCut=0 for no width cut

ClusteringAlgorithm_broadside_PixelDigi_ = cms.ESProducer("ClusteringAlgorithm_broadside_PixelDigi_" , WidthCut = cms.int32(3)) # WidthCut=0 for no width cut

ClusteringAlgorithm_broadside_TTHit_ = cms.ESProducer("ClusteringAlgorithm_broadside_TTHit_" , WidthCut = cms.int32(3)) # WidthCut=0 for no width cut
"""


"""ClusteringAlgorithm_2d_PSimHit_ = cms.ESProducer("ClusteringAlgorithm_2d_PSimHit_" , DoubleCountingTest=cms.bool(False) )

ClusteringAlgorithm_2d_PixelDigi_ = cms.ESProducer("ClusteringAlgorithm_2d_PixelDigi_" , DoubleCountingTest=cms.bool(True) )

ClusteringAlgorithm_2d_TTHit_ = cms.ESProducer("ClusteringAlgorithm_2d_TTHit_" , DoubleCountingTest=cms.bool(True) )
"""
