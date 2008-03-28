import FWCore.ParameterSet.Config as cms

jpsieeEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring()
)
jpsieeEventSelection = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('jpsiee_path')
    )
)

