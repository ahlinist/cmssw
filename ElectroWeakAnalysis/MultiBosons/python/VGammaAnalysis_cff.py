import FWCore.ParameterSet.Config as cms

#Z -> l l  gamma
import ElectroWeakAnalysis.MultiBosons.ZMuMuGamma_cff as ZMuMuGamma
import ElectroWeakAnalysis.MultiBosons.ZEEGamma_cff as ZEEGamma

#Z -> nu nu gamma

#W -> l nu gamma


VGammaAnalysis = cms.Sequence(ZMuMuGamma.ZMuMuGammaWorkflow+
                              ZEEGamma.ZEEGammaWorkflow)
                             

