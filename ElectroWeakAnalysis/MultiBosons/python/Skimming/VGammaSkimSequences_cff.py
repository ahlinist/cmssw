import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Skimming.VGammaCombinedCandidates_cfi import *
from ElectroWeakAnalysis.MultiBosons.Skimming.VGammaFilters_cfi import *

WENuGammaSequence  = cms.Sequence(electronPlusMETs * WENuGammaCands * WENuGammaFilter)
WMuNuGammaSequence = cms.Sequence(muonPlusMETs * WMuNuGammaCands * WMuNuGammaFilter)
ZEEGammaSequence = cms.Sequence(dielectrons * ZEEGammaCands * ZEEGammaFilter)
ZMuMuGammaSequence = cms.Sequence(dimuons * ZMuMuGammaCands * ZMuMuGammaFilter)
ZNuNuGammaSequence = cms.Sequence(ZNuNuGammaCands * ZNuNuGammaFilter)
