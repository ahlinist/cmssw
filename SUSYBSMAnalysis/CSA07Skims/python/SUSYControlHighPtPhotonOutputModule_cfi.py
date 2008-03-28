import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
SUSYControlHighPtPhotonOutputModule = cms.OutputModule("PoolOutputModule",
    AODSIMEventContent,
    SelectEvents = cms.untracked.PSet(
        dataTier = cms.untracked.string('USER'),
        SelectEvents = cms.vstring('SUSYControlHighPtPhoton')
    ),
    fileName = cms.untracked.string('SUSYControlHighPtPhoton.root')
)


