import FWCore.ParameterSet.Config as cms

ecalPreshowerRecHit.ESdigiCollection = 'esZeroSuppression'
ecalWeightUncalibRecHit.EBdigiCollection = cms.InputTag("ecalSelectiveReadout","ebDigis")
ecalWeightUncalibRecHit.EEdigiCollection = cms.InputTag("ecalSelectiveReadout","eeDigis")

