import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PFSkims.jpsiee_EventContent_cff import *
from RecoParticleFlow.PFSkims.FEVTSIMJPsieeEventContent_cff import *
jpsieeOutputModuleFEVTSIM = cms.OutputModule("PoolOutputModule",
    FEVTSIMJPsieeEventContent,
    jpsieeEventSelection,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('jpsieeFEVTSIM'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('jpsieeFEVTSIM.root')
)


