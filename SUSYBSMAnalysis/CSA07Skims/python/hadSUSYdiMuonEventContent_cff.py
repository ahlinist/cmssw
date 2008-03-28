import FWCore.ParameterSet.Config as cms

# Can insert block to customize what goes inside root file on top of AOD/RECO
hadSUSYdiMuonEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring()
)
hadSUSYdiMuonEventSelection = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('hadSUSYdiMuon')
    )
)

