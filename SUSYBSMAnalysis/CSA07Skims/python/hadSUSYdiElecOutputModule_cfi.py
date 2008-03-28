import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.hadSUSYdiElecEventContent_cff import *
hadSUSYdiElecOutputModule = cms.OutputModule("PoolOutputModule",
    hadSUSYdiElecEventSelection,
    AODSIMEventContent,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('hadSUSYdiElec'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('hadSUSYdiElec.root')
)


