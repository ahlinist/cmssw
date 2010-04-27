import FWCore.ParameterSet.Config as cms

#W -> l nu gamma

#Z -> l l  gamma
from ElectroWeakAnalysis.MultiBosons.ZMuMuGamma_cff import *
#from ElectroWeakAnalysis.MultiBosons.ZEEGamma_cff import *

#Z -> nu nu gamma


VGammaAnalysis = cms.Sequence(ZMuMuGammaWorkflow)#+
#                              ZEEGammaWorkflow)
                             

