import FWCore.ParameterSet.Config as cms

goodMuons = cms.EDFilter("CandViewRefSelector",
  src = cms.InputTag("muons"),
  cut = cms.string("pt > 15 & (isGlobalMuon | isTrackerMuon)"),
  filter = cms.bool(True)
  )

goodPhotons = cms.EDFilter("CandViewRefSelector",
  src = cms.InputTag("photons"),
  cut = cms.string("pt > 10"),
  filter = cms.bool(True)
  )

muonPhotonSkimFilterSequence = cms.Sequence(
  goodMuons *
  goodPhotons
  )
