import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Skimming.hltFilter_cfi import *
from ElectroWeakAnalysis.MultiBosons.Skimming.VGammaCombinedCandidates_cfi import *
from ElectroWeakAnalysis.MultiBosons.Skimming.VGammaFilters_cfi import *
from ElectroWeakAnalysis.MultiBosons.Skimming.GenPhotonFilters_cfi import *


WENuGammaSequence  = cms.Sequence(
  hltFilter +
  electronPlusMETs *
  electronPlusMETFilter *
  WENuGammaCands *
  WENuGammaFilter
  )

WMuNuGammaSequence = cms.Sequence(
  hltFilter +
  muonPlusMETs *
  muonPlusMETFilter *
  WMuNuGammaCands *
  WMuNuGammaFilter
  )

ZEEGammaSequence = cms.Sequence(
  hltFilter +
  dielectrons *
  dielectronFilter *
  ZEEGammaCands *
  ZEEGammaFilter
  )

ZMuMuGammaSequence = cms.Sequence(
  hltFilter +
  dimuons *
  dimuonFilter *
  ZMuMuGammaCands *
  ZMuMuGammaFilter
  )

ZInvisibleGammaSequence = cms.Sequence(
  hltFilter +
  ZInvisibleGammaCands *
  ZInvisibleGammaFilter
  )

pythiaPartonShowerIsrSequence = cms.Sequence(
  hltFilter +
  leadingPhoton *
  pythiaPartonShowerIsrFilter
  )

pythiaPartonShowerFsrSequence = cms.Sequence(
  hltFilter +
  leadingPhoton *
  pythiaPartonShowerFsrFilter
  )
