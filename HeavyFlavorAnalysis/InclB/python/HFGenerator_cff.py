import FWCore.ParameterSet.Config as cms

from SimGeneral.HepPDTESSource.pythiapdt_cfi import *
genDump = cms.EDAnalyzer("HFDumpGenerator",
    GenEventScale = cms.untracked.string('genEventScale'),
    generatorCandidates = cms.untracked.string('genParticles'),
    GenEventProcID = cms.untracked.string('genEventProcID'),
    GenEventWeight = cms.untracked.string('genEventWeight'),
    verbose = cms.untracked.int32(0),
    generatorEvent = cms.untracked.string('source')
)

GenDump = cms.Sequence(genDump)

