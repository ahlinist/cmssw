import FWCore.ParameterSet.Config as cms

SeedComparitorWithPixelStubs = cms.PSet(
    ComponentName = cms.string('SeedComparitorWithPixelStubs'),
    tempCutFactor = cms.double(1E-9),
    
    TanLorentzAnglePerTesla = cms.double(0.106),
    Alpha2Order = cms.bool(True),
    speed = cms.int32(3),
    ClusterProbComputationFlag = cms.int32(0),
    DoCosmics = cms.bool(False),
    LoadTemplatesFromDB = cms.bool(True),
    UseClusterSplitter = cms.bool(False)
    )


