import FWCore.ParameterSet.Config as cms

GamIsoTrackExtractorBlock = cms.PSet(
    ComponentName = cms.string('EgammaTrackExtractorVtx'),
    inputTrackCollection = cms.InputTag("generalTracks"),
    DepositLabel = cms.untracked.string(''),
    Diff_r = cms.double(0.2),
    Diff_z = cms.double(1.0),
    DR_Max = cms.double(1.0),
    DR_Veto = cms.double(0.0),

    BeamlineOption = cms.string('UseVertex'),
    BeamSpotLabel = cms.InputTag("offlineBeamSpot"),
    NHits_Min = cms.uint32(0),
    Chi2Ndof_Max = cms.double(1e+64),
    Chi2Prob_Min = cms.double(-1.0),
    Pt_Min = cms.double(-1.0),

    dzOption = cms.string("vtx")
)

