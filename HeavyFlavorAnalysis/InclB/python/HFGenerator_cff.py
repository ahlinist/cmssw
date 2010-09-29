import FWCore.ParameterSet.Config as cms

from SimGeneral.HepPDTESSource.pythiapdt_cfi import *

genDump = cms.EDAnalyzer("HFDumpGenerator",
    generatorCandidates = cms.untracked.string('genParticles'),
    gencandstat         = cms.untracked.int32(0),
    gencandptmin        = cms.untracked.double(1.0),
    verbose = cms.untracked.int32(0),
    generatorEvent = cms.untracked.string('source')
)

GenDump = cms.Sequence(genDump)

