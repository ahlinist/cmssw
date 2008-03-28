import FWCore.ParameterSet.Config as cms

TrackerSimHits = cms.PSet(
    # Smallest charged particle pT for which SimHit's are saved (GeV/c)
    pTmin = cms.untracked.double(0.3),
    # Save SimHit's only for the first loop
    firstLoop = cms.untracked.bool(True)
)

