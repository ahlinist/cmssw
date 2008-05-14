import FWCore.ParameterSet.Config as cms

# Take all pixel tracks but the potential electrons
hltL1IsoElectronsRegionalCTFFinalFitWithMaterial = cms.EDFilter("FastTrackMerger",
    SaveTracksOnly = cms.untracked.bool(True),
    TrackProducers = cms.VInputTag(cms.InputTag("globalPixelWithMaterialTracks"))
)

hltL1IsoStartUpElectronsRegionalCTFFinalFitWithMaterial = cms.EDFilter("FastTrackMerger",
    SaveTracksOnly = cms.untracked.bool(True),
    TrackProducers = cms.VInputTag(cms.InputTag("globalPixelWithMaterialTracks"))
)

# The sequence
HLTL1IsoElectronsRegionalRecoTrackerSequence = cms.Sequence(cms.SequencePlaceholder("globalPixelTracking")+hltL1IsoElectronsRegionalCTFFinalFitWithMaterial)
HLTL1IsoStartUpElectronsRegionalRecoTrackerSequence = cms.Sequence(cms.SequencePlaceholder("globalPixelTracking")+hltL1IsoStartUpElectronsRegionalCTFFinalFitWithMaterial)

