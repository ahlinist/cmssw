import FWCore.ParameterSet.Config as cms

# Particle Flow FEVTSIM event content 
# Saves standard FEVTSIM and PF-specific data (which might not be empty)
#
from Configuration.EventContent.EventContent_cff import *
from RecoParticleFlow.PFSkims.jpsiee_EventContent_cff import *
FEVTSIMJPsieeEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring()
)
FEVTSIMJPsieeEventContent.outputCommands.extend(FEVTSIMEventContent.outputCommands)
FEVTSIMJPsieeEventContent.outputCommands.extend(jpsieeEventContent.outputCommands)

