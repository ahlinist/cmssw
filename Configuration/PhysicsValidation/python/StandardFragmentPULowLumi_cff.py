import FWCore.ParameterSet.Config as cms

#
# common part between all the Physics validation pieces
#
from Configuration.StandardSequences.Reconstruction_cff import *
from Configuration.StandardSequences.Simulation_cff import *
from Configuration.StandardSequences.MixingLowLumiPileUp_cff import *
from Configuration.StandardSequences.VtxSmearedGauss_cff import *
from Configuration.StandardSequences.FakeConditions_cff import *
# Event output
from Configuration.EventContent.EventContent_cff import *
FEVT = cms.OutputModule("PoolOutputModule",
    FEVTSIMEventContent,
    datasets = cms.untracked.PSet(
        dataset1 = cms.untracked.PSet(
            dataTier = cms.untracked.string('FEVT')
        )
    ),
    fileName = cms.untracked.string('file:c_jets.root')
)

p1 = cms.Path(simulation)
p2 = cms.Path(reconstruction) ## reconstruction

outpath = cms.EndPath(FEVT)

