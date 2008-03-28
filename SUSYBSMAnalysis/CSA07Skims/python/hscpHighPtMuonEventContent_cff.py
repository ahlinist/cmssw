import FWCore.ParameterSet.Config as cms

hscpHighPtMuonEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_standAloneMuons_*_*', 'keep *_globalMuons_*_*', 'keep *_dt1DRecHits_*_*', 'keep *_dt2DSegments_*_*', 'keep *_dt4DSegments_*_*', 'keep *_ctfWithMaterialTracks_*_*', 'keep *_TrackRefitter_*_*', 'keep *_dedxHitsFromRefitter_*_*')
)
hscpHighPtMuonEventSelection = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('hscpHighPtMuon')
    )
)

