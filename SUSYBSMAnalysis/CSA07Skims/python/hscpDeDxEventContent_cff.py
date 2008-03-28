# The following comments couldn't be translated into the new config version:

#       "keep *_ctfWithMaterialTracks_*_*",

import FWCore.ParameterSet.Config as cms

hscpDeDxEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_standAloneMuons_*_*', 'keep *_globalMuons_*_*', 'keep *_dt1DRecHits_*_*', 'keep *_dt2DSegments_*_*', 'keep *_dt4DSegments_*_*', 'keep recoTracks_TrackRefitter_*_*', 'keep *_dedxHitsFromRefitter_*_*', 'keep *_dedxTruncated40_*_*')
)
hscpDeDxEventSelection = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('hscpDeDx')
    )
)

