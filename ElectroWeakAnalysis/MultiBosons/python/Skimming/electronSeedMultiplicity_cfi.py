import FWCore.ParameterSet.Config as cms

## stuff for the z->mmg pixel match veto efficiency measurement
electronSeedMultiplicity = cms.EDProducer('ElectronSeedCounter',
    sources = cms.VPSet(
        cms.PSet( src = cms.InputTag('electronMergedSeeds') ),
    )
)
