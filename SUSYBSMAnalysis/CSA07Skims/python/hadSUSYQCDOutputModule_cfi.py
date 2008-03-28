import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.hadSUSYQCDControlMETEventContent_cff import *
hadSUSYQCDControlMETOutputModule = cms.OutputModule("PoolOutputModule",
    hadSUSYQCDControlMETEventSelection,
    AODSIMEventContent,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('hadSUSYQCDControlMET'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('hadSUSYQCDControlMET.root')
)


