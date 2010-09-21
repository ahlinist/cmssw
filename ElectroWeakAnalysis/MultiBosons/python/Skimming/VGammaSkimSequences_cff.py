import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Skimming.VGammaCombinedCandidates_cfi import *
from ElectroWeakAnalysis.MultiBosons.Skimming.VGammaFilters_cfi import *
from ElectroWeakAnalysis.MultiBosons.Skimming.GenPhotonFilters_cfi import *


WENuGammaSequence  = cms.Sequence(
  electronPlusMETs *
  electronPlusMETFilter *
  WENuGammaCands *
  WENuGammaFilter
  )

WMuNuGammaSequence = cms.Sequence(
  muonPlusMETs *
  muonPlusMETFilter *
  WMuNuGammaCands *
  WMuNuGammaFilter
  )

ZEEGammaSequence = cms.Sequence(
  dielectrons *
  dielectronFilter *
  ZEEGammaCands *
  ZEEGammaFilter
  )

ZMuMuGammaSequence = cms.Sequence(
  dimuons *
  dimuonFilter *
  ZMuMuGammaCands *
  ZMuMuGammaFilter
  )

ZInvisibleGammaSequence = cms.Sequence(
  ZInvisibleGammaCands *
  ZInvisibleGammaFilter
  )

pythiaPartonShowerIsrSequence = cms.Sequence(
  leadingPhoton *
  pythiaPartonShowerIsrFilter
  )

pythiaPartonShowerFsrSequence = cms.Sequence(
  leadingPhoton *
  pythiaPartonShowerFsrFilter
  )
