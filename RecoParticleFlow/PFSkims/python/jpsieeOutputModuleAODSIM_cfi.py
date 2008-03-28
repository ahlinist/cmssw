import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PFSkims.jpsiee_EventContent_cff import *
from RecoParticleFlow.PFSkims.AODSIMJPsieeEventContent_cff import *
jpsieeOutputModuleAODSIM = cms.OutputModule("PoolOutputModule",
    AODSIMJPsieeEventContent,
    jpsieeEventSelection,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('jpsieeAODSIM'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('jpsieeAODSIM.root')
)


