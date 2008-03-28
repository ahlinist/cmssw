import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
SUSYHighPtPhotonOutputModule = cms.OutputModule("PoolOutputModule",
    AODSIMEventContent,
    SelectEvents = cms.untracked.PSet(
        dataTier = cms.untracked.string('USER'),
        SelectEvents = cms.vstring('SUSYHighPtPhoton')
    ),
    fileName = cms.untracked.string('SUSYHighPtPhoton.root')
)


