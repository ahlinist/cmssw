import FWCore.ParameterSet.Config as cms

hlt30v1 = cms.EDFilter("HLTHighLevel",
  TriggerResultsTag  = cms.InputTag('TriggerResults','','HLT'),
  HLTPaths           = cms.vstring('HLT_Jet30_v1'),
  eventSetupPathsKey = cms.string(''),
  andOr              = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
  throw              = cms.bool(True)
)

hlt60v1  = hlt30v1.clone(HLTPaths = cms.vstring('HLT_Jet60_v1'))
hlt80v1  = hlt30v1.clone(HLTPaths = cms.vstring('HLT_Jet80_v1'))
hlt110v1 = hlt30v1.clone(HLTPaths = cms.vstring('HLT_Jet110_v1'))
hlt150v1 = hlt30v1.clone(HLTPaths = cms.vstring('HLT_Jet150_v1'))
hlt190v1 = hlt30v1.clone(HLTPaths = cms.vstring('HLT_Jet190_v1'))
hlt240v1 = hlt30v1.clone(HLTPaths = cms.vstring('HLT_Jet240_v1'))
hlt370v1 = hlt30v1.clone(HLTPaths = cms.vstring('HLT_Jet370_v1'))
