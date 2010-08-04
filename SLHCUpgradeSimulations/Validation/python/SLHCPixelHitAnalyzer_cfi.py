import FWCore.ParameterSet.Config as cms

SLHCPixelHitAnalyzer = cms.EDFilter("SLHCPixelHitAnalyzer",
    trajectoryInput = cms.string('TrackRefitter'),
    useAllPixel = cms.bool(False),
    isCosmic = cms.bool(True),
    isSim = cms.bool(False),

# should be moved to TFileService.                                       
    OutputFile = cms.string('TTreeFile.root') 
)

