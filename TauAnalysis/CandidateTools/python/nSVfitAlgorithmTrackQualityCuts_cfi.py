import FWCore.ParameterSet.Config as cms

trackQualityCuts = cms.PSet(
    minNumHits = cms.uint32(5),
    minNumPixelHits = cms.uint32(1),
    maxChi2DoF = cms.double(10.),
    maxDeltaPoverP = cms.double(1.e+3),
    minPt = cms.double(2.)
)    
