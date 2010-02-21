import FWCore.ParameterSet.Config as cms

promptanaPMTnoise = cms.EDProducer("PromptAna_PMTNoise",
				   RecHitTag=cms.InputTag("hfreco"),
				   METTag=cms.InputTag("met"),
				   TrackTag=cms.InputTag("generalTracks"),
				   prefix=cms.string("promptanaPMTnoise"),
				   suffix=cms.string(""),
				   isMC=cms.bool(False),
				   HLTriggerResults=cms.InputTag("TriggerResults","","HLT"),
				   L1Triggers=cms.InputTag("gtDigis"),
                                   debug=cms.untracked.int32(False))
