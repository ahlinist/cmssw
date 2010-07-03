import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Skimming.VGammaCombinedCandidates_cfi import *
from ElectroWeakAnalysis.MultiBosons.Skimming.VGammaFilters_cfi import *

WENuGammaSequence  = cms.Sequence(
  electronPlusMETs * WENuGammaCands * electronPlusMETFilter * WENuGammaFilter
    )

WMuNuGammaSequence = cms.Sequence(
  muonPlusMETs * WMuNuGammaCands * muonPlusMETFilter * WMuNuGammaFilter
    )

ZEEGammaSequence = cms.Sequence(
  dielectrons * ZEEGammaCands * dielectronFilter * ZEEGammaFilter
    )

ZMuMuGammaSequence = cms.Sequence(
  dimuons * ZMuMuGammaCands * dimuonFilter * ZMuMuGammaFilter
    )

ZInvisibleGammaSequence = cms.Sequence(ZInvisibleGammaCands * ZInvisibleGammaFilter)
