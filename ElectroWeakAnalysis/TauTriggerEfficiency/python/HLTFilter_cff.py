import FWCore.ParameterSet.Config as cms

IncludedHLTs = cms.EDFilter("HLTFilter",
	HLTResults   = cms.InputTag("TriggerResults::HLT"),
	HLTSelection = cms.VInputTag(cms.InputTag("HLT_L1MuOpen"),
                                     cms.InputTag("HLT_L1Mu"),
                                     cms.InputTag("HLT_L2Mu9"),
                                     cms.InputTag("HLT_L2Mu11"),
                                     cms.InputTag("HLT_Mu3"),
                                     cms.InputTag("HLT_Mu5"),
                                     cms.InputTag("HLT_Mu7"),
                                     cms.InputTag("HLT_Mu9"),
                                     cms.InputTag("HLT_Mu11"),
                                     cms.InputTag("HLT_Mu5"),
                                     cms.InputTag("HLT_DoubleMu0"),
                                     cms.InputTag("HLT_DoubleMu3"),
                                     cms.InputTag("HLT_DoubleMu5"),
                                     cms.InputTag("HLT_L1DoubleMuOpen")
	)
)
