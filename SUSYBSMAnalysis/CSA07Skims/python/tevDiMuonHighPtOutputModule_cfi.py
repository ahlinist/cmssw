import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.tevDiMuonHighPtEventContent_cff import *
tevDiMuonHighPtOutputModule = cms.OutputModule("PoolOutputModule",
    tevDiMuonHighPtEventContent,
    tevDiMuonHighPtEventSelection,
    dataset = cms.untracked.PSet(
        filterName = cms.string('tevDiMuonHighPt'),
        dataTier = cms.string('USER')
    ),
    fileName = cms.untracked.string('TeVdiMuonHighPt.root')
)


