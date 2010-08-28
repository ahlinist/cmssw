import FWCore.ParameterSet.Config as cms
from SimTracker.TrackAssociation.LhcParametersDefinerForTP_cfi import *

SLHCPixelHitAnalyzer = cms.EDFilter("SLHCPixelHitAnalyzer",
    trajectoryInput = cms.string('TrackRefitter'),
    useAllPixel = cms.bool(False),
    isCosmic = cms.bool(False),
    isSim = cms.bool(True),
    trackProducer = cms.InputTag("TrackRefitter"),
    associator = cms.string('TrackAssociatorByHits'),
    label_tp = cms.InputTag("mergedtruth","MergedTrackTruth"),
    # collision like tracks
    parametersDefiner = cms.string('LhcParametersDefinerForTP'),
    beamSpotLabel = cms.InputTag("offlineBeamSpot"),

# moved to TFileService.                                       
    OutputFile = cms.string('TTreeFile.root') 
)

