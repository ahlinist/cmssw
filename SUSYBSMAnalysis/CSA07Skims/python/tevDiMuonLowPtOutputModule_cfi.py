import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.tevDiMuonLowPtEventContent_cff import *
tevDiMuonLowPtOutputModule = cms.OutputModule("PoolOutputModule",
    tevDiMuonLowPtEventSelection,
    tevDiMuonLowPtEventContent,
    dataset = cms.untracked.PSet(
        filterName = cms.string('tevDiMuonLowPt'),
        dataTier = cms.string('USER')
    ),
    fileName = cms.untracked.string('TeVdiMuonLowPt.root')
)


