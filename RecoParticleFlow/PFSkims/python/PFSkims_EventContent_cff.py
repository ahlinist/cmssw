import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PFSkims.jpsiee_EventContent_cff import *
PFSkimsEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring()
)
PFSkimsEventContent.outputCommands.extend(jpsieeEventContent.outputCommands)

