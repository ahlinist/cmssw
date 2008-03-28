import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.hadSUSYTopMuonEventContent_cff import *
hadSUSYTopMuonOutputModule = cms.OutputModule("PoolOutputModule",
    hadSUSYTopMuonEventSelection,
    AODSIMEventContent,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('hadSUSYTopMuon'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('hadSUSYTopMuon.root')
)


