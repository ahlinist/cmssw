import FWCore.ParameterSet.Config as cms

IncludedHLTs = cms.EDFilter("HLTFilter",
	HLTResults   = cms.InputTag("TriggerResults::HLT"),
	HLTSelection = cms.VInputTag(cms.InputTag("HLT_L1Jet15"),
                                     cms.InputTag("HLT_Jet30"),
                                     cms.InputTag("HLT_Jet50"),
                                     cms.InputTag("HLT_Jet50"),
                                     cms.InputTag("HLT_Jet80"),
                                     cms.InputTag("HLT_Jet110"),
                                     cms.InputTag("HLT_Jet180"),
	                             cms.InputTag("HLT_Jet250"),
                                     cms.InputTag("HLT_FwdJet20"),
                                     cms.InputTag("HLT_DiJetAve15"),
                                     cms.InputTag("HLT_DiJetAve30"),
                                     cms.InputTag("HLT_DiJetAve50"),
                                     cms.InputTag("HLT_DiJetAve70"),
                                     cms.InputTag("HLT_DiJetAve130"),
                                     cms.InputTag("HLT_DiJetAve220")
	)
)
