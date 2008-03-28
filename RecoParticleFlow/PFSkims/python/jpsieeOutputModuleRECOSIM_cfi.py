import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PFSkims.jpsiee_EventContent_cff import *
from RecoParticleFlow.PFSkims.RECOSIMJPsieeEventContent_cff import *
jpsieeOutputModuleRECOSIM = cms.OutputModule("PoolOutputModule",
    RECOSIMJPsieeEventContent,
    jpsieeEventSelection,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('jpsieeRECOSIM'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('jpsieeRECOSIM.root')
)


