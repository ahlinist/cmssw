import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.triggerResultsFilter_cfi as hlt

MuonHLTs = hlt.triggerResultsFilter.clone(
    hltResults    = cms.InputTag('TriggerResults::HLT'),
    triggerConditions =  (
#        'HLT_L1MuOpen',
#        'HLT_L1Mu',
#        'HLT_L2Mu9',
#        'HLT_L2Mu11',
        'HLT_Mu3',
        'HLT_Mu5',
        'HLT_Mu7',
        'HLT_Mu9',
        'HLT_Mu11'
#        'HLT_DoubleMu0',
#        'HLT_DoubleMu3',
#        'HLT_DoubleMu5',
#        'HLT_L1DoubleMuOpen'
    ),
    l1tResults = '',
    throw = False
)

HLT_LooseIsoPFTau35_Trk20_Prong1 = hlt.triggerResultsFilter.clone(
    hltResults    = cms.InputTag('TriggerResults::HLT'),
    triggerConditions =  (
	'HLT_LooseIsoPFTau35_Trk20_Prong1_v1',
	'HLT_LooseIsoPFTau35_Trk20_Prong1_v2',
	'HLT_LooseIsoPFTau35_Trk20_Prong1_v3',
	'HLT_LooseIsoPFTau35_Trk20_Prong1_v4',
	'HLT_LooseIsoPFTau35_Trk20_Prong1_v5',
        'HLT_LooseIsoPFTau35_Trk20_Prong1_v6',
	'HLT_LooseIsoPFTau35_Trk20_Prong1_v7'
    ),
    l1tResults = '',
    throw = False
)

#HLTFilter code located in TTEff
#IncludedHLTs = cms.EDFilter("HLTFilter",
#	HLTResults   = cms.InputTag("TriggerResults::HLT"),
#	HLTSelection = cms.VInputTag(cms.InputTag("HLT_L1MuOpen"),
#                                     cms.InputTag("HLT_L1Mu"),
#                                     cms.InputTag("HLT_L2Mu9"),
#                                     cms.InputTag("HLT_L2Mu11"),
#                                     cms.InputTag("HLT_Mu3"),
#                                     cms.InputTag("HLT_Mu5"),
#                                     cms.InputTag("HLT_Mu7"),
#                                     cms.InputTag("HLT_Mu9"),
#                                     cms.InputTag("HLT_Mu11"),
#                                     cms.InputTag("HLT_Mu5"),
#                                     cms.InputTag("HLT_DoubleMu0"),
#                                     cms.InputTag("HLT_DoubleMu3"),
#                                     cms.InputTag("HLT_DoubleMu5"),
#                                     cms.InputTag("HLT_L1DoubleMuOpen")
#	)
#)
