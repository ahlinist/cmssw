import FWCore.ParameterSet.Config as cms

import ElectroWeakAnalysis.MultiBosons.ZMuMuGamma_cff as ZMuMuGamma
import ElectroWeakAnalysis.MultiBosons.ZEEGamma_cff as ZEEGamma

VGammaAnalysis = cms.Sequence(ZMuMuGamma.ZMuMuGammaWorkflow+
                              ZEEGamma.ZEEGammaWorkflow)
                             

