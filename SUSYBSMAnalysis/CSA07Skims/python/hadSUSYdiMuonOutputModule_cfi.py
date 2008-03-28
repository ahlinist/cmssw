import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.hadSUSYdiMuonEventContent_cff import *
hadSUSYdiMuonOutputModule = cms.OutputModule("PoolOutputModule",
    hadSUSYdiMuonEventSelection,
    AODSIMEventContent,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('hadSUSYdiMuon'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('hadSUSYdiMuon.root')
)


