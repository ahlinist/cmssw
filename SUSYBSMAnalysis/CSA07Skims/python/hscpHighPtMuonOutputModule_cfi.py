import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.hscpHighPtMuonEventContent_cff import *
hscpHighPtMuonOutputModule = cms.OutputModule("PoolOutputModule",
    hscpHighPtMuonEventContent,
    AODSIMEventContent,
    hscpHighPtMuonOutputContent,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('hscpHighPtMuon'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('hscpHighPtMuon.root')
)


