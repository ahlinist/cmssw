import FWCore.ParameterSet.Config as cms

triggerHistManager = cms.PSet(
  name = cms.string('triggerHistManager'),
  type = cms.string('TriggerHistManager'),

  l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
  l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
  l1Bits = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                       'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15',
                       'L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14'),

  hltResultsSource = cms.InputTag('TriggerResults::HLT'),
  hltPaths = cms.vstring('HLT_IsoEle15_L1I',
                         'HLT_Mu15', 'HLT_IsoMu11'),

  dqmDirectory_store = cms.string('TriggerQuantities')
)
