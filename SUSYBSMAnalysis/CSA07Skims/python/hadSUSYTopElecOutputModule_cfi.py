import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.hadSUSYTopElecEventContent_cff import *
hadSUSYTopElecOutputModule = cms.OutputModule("PoolOutputModule",
    hadSUSYTopElecEventSelection,
    AODSIMEventContent,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('hadSUSYTopElec'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('hadSUSYTopElec.root')
)


