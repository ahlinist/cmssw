import FWCore.ParameterSet.Config as cms

from RecoEgamma.ElectronIdentification.electronIdCutBasedExt_cfi import *

eidCutBasedExt.robusttightEleIDCuts = cms.PSet(
    barrel = cms.vdouble(0.01, 0.012, 0.025, 0.0040, -1., -1.), # default: 0.01, 0.0099, 0.025, 0.0040, -1., -1.
    endcap = cms.vdouble(0.01, 0.028, 0.020, 0.0066, -1., -1.)  # default: 0.01, 0.028,  0.020, 0.0066, -1., -1.
)
