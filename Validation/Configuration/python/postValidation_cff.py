import FWCore.ParameterSet.Config as cms

from Validation.RecoMuon.PostProcessor_cff import *
from Validation.RecoTrack.PostProcessorTracker_cfi import *
from Validation.MuonIsolation.PostProcessor_cff import *
from Validation.CaloTowers.CaloTowersPostProcessor_cff import *
from Validation.HcalRecHits.hcalRecHitsPostProcessor_cff import *


postValidation = cms.Sequence(
      recoMuonPostProcessors
    + postProcessorTrack
    + MuIsoValPostProcessor
    + calotowersPostProcessor
    + hcalrechitsPostProcessor
)


postValidation_fastsim = cms.Sequence(
      recoMuonPostProcessorsFastSim
    + postProcessorTrack
    + MuIsoValPostProcessor
)
