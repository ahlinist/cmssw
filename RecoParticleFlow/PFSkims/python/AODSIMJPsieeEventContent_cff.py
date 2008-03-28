import FWCore.ParameterSet.Config as cms

# Particle Flow AODSIM event content 
# Saves standard AODSIM and PF-specific data (which might not be empty)
#
from Configuration.EventContent.EventContent_cff import *
from RecoParticleFlow.PFSkims.jpsiee_EventContent_cff import *
AODSIMJPsieeEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring()
)
AODSIMJPsieeEventContent.outputCommands.extend(AODSIMEventContent.outputCommands)
AODSIMJPsieeEventContent.outputCommands.extend(jpsieeEventContent.outputCommands)

