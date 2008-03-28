import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.hadSUSYSMBackgrEventContent_cff import *
hadSUSYSMBackgrOutputModule = cms.OutputModule("PoolOutputModule",
    hadSUSYSMBackgrEventSelection,
    AODSIMEventContent,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('hadSUSYSMBackgr'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('hadSUSYSMBackgr.root')
)


