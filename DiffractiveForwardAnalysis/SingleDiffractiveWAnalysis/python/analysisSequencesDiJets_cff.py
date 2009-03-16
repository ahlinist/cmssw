import FWCore.ParameterSet.Config as cms

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.genParticlesCalo_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.edmDump_cff import *

pileUpInfo.AccessCrossingFramePlayBack = True
pileUpInfo.BunchCrossings = cms.vint32(0)

edmDumpSequence = cms.Sequence(genParticlesCalo*edmDumpAllNoPileUp)
