import FWCore.ParameterSet.Config as cms

SLHCAllPixelHitAnalyzer = cms.EDFilter("SLHCAllPixelHitAnalyzer",
    src = cms.InputTag("siPixelRecHits"),
    trajectoryInput = cms.string('TrackRefitter'),
    useAllPixel = cms.bool(False),
    isCosmic = cms.bool(True),
    isSim = cms.bool(False),
    trackProducer = cms.InputTag("ctfWithMaterialTracks"),
    ### for using track hit association
   associatePixel = cms.bool(True),
   associateStrip = cms.bool(False),
   associateRecoTracks = cms.bool(False),
   ROUList = cms.vstring('g4SimHitsTrackerHitsPixelBarrelLowTof',
                         'g4SimHitsTrackerHitsPixelBarrelHighTof',
                         'g4SimHitsTrackerHitsPixelEndcapLowTof',
                         'g4SimHitsTrackerHitsPixelEndcapHighTof'),

# should be moved to TFileService.                                       
    OutputFile = cms.string('AllTTreeFile.root') 
)

