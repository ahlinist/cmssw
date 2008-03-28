import FWCore.ParameterSet.Config as cms

# Save RECO, SIM and possibel extras
from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.HEEPSkimEventContent_cff import *
RECOSIMHEEPSkimEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring()
)
RECOSIMHEEPSkimEventContent.outputCommands.extend(RECOSIMEventContent.outputCommands)
RECOSIMHEEPSkimEventContent.outputCommands.extend(hEEPSkimEventContent.outputCommands)

