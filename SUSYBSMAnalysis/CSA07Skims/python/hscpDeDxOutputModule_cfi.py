import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.hscpDeDxEventContent_cff import *
hscpDeDxOutputModule = cms.OutputModule("PoolOutputModule",
    hscpDeDxEventContent,
    AODSIMEventContent,
    hscpDeDxOutputContent,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('hscpDeDxFilter'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('hscpHighPtMuon.root')
)


