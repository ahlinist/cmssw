import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
tevDiMuonLowPtEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring()
)
tevDiMuonLowPtEventSelection = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('tevDiMuonLowPt')
    )
)
tevDiMuonLowPtEventContent.outputCommands.extend(AODSIMEventContent.outputCommands)

