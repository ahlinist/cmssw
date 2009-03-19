import FWCore.ParameterSet.Config as cms

from RecoLocalMuon.DTRecHit.DTLinearDriftAlgo_CosmicData_cfi import *

dt4DSegmentsT0Corr = cms.EDProducer("DTSegment4DT0Corrector",
                                    DTLinearDriftAlgo_CosmicData,
                                    recHits4DLabel = cms.InputTag("dt4DSegments"),
                                    debug = cms.untracked.bool(False),
                                    performT0_vdriftSegCorrection = cms.bool(True),
                                    hit_afterT0_resolution = cms.double(0.03)
)
