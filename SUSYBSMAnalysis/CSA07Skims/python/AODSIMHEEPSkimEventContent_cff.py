import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.HEEPSkimEventContent_cff import *
AODSIMHEEPSkimEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring()
)
AODSIMHEEPSkimEventContent.outputCommands.extend(AODSIMEventContent.outputCommands)
AODSIMHEEPSkimEventContent.outputCommands.extend(hEEPSkimEventContent.outputCommands)

