import FWCore.ParameterSet.Config as cms

from IOMC.EventVertexGenerators.VtxSmearedParameters_cfi import *
myVertexGenerator = cms.PSet(
    GaussVtxSmearingParameters,
    type = cms.string('Gaussian')
)

