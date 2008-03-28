import FWCore.ParameterSet.Config as cms

# Particle Flow RECOSIM event content 
# Saves standard RECOSIM and PF-specific data (which might not be empty)
#
from Configuration.EventContent.EventContent_cff import *
from RecoParticleFlow.PFSkims.jpsiee_EventContent_cff import *
RECOSIMJPsieeEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring()
)
RECOSIMJPsieeEventContent.outputCommands.extend(RECOSIMEventContent.outputCommands)
RECOSIMJPsieeEventContent.outputCommands.extend(jpsieeEventContent.outputCommands)

