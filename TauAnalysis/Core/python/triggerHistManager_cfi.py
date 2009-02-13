import FWCore.ParameterSet.Config as cms

triggerHistManager = cms.PSet(
  name = cms.string('triggerHistManager'),
  type = cms.string('TriggerHistManager'),

  l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
  l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
  l1Bits = cms.vstring(''),

  hltResultsSource = cms.InputTag('TriggerResults::HLT'),
  hltPaths = cms.vstring(''),

  dqmDirectory_store = cms.string('TriggerQuantities')
)
