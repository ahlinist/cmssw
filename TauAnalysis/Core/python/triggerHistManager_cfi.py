import FWCore.ParameterSet.Config as cms

triggerHistManager = cms.PSet(
    pluginName = cms.string('triggerHistManager'),
    pluginType = cms.string('TriggerHistManager'),

    # L1 trigger bits not contained in AOD;
    # in order to process Monte Carlo samples produced by FastSimulation,
    # disable histogram filling for now
    l1GtReadoutRecordSource = cms.InputTag(''),
    l1GtObjectMapRecordSource = cms.InputTag(''),
    #l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
    #l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
    l1Bits = cms.vstring(''),

    hltResultsSource = cms.InputTag('TriggerResults::HLT'),
    hltPaths = cms.vstring(''),

    dqmDirectory_store = cms.string('TriggerQuantities')
)
