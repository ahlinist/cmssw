import FWCore.ParameterSet.Config as cms

SeedComparitorPSet = cms.PSet(
    ComponentName = cms.string('SeedComparitorWithPixelStubs'),
    tempCutFactor = cms.double(1E-9)
)


