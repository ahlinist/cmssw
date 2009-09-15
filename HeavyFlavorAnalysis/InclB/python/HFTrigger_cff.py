import FWCore.ParameterSet.Config as cms

triggerDump = cms.EDFilter("HFDumpTrigger",
    verbose = cms.untracked.int32(0),
    GlobalTriggerLabel = cms.untracked.string('hltGtDigis'),
    L1MapLabel = cms.untracked.string('hltL1GtObjectMap'),
    L1MuLabel = cms.untracked.string('hltL1extraParticles'),
    L1TriggerName = cms.untracked.string('L1_SingleMu7'),
    L1w1 = cms.untracked.string('L1_SingleMu3'),
    L1w2 = cms.untracked.string('L1_SingleMu5'),
    L1w3 = cms.untracked.string('L1_SingleMu7'),
    L1w4 = cms.untracked.string('L1_SingleMu10'),
    hltLabel = cms.InputTag("TriggerResults","","HLT"),
    hltobjectLabel = cms.untracked.string('hltTriggerSummaryAOD'),                      
    HLTriggerName = cms.untracked.string('HLT_Mu9'),
    HLTw1 = cms.untracked.string('HLT_Mu3'),
    HLTw2 = cms.untracked.string('HLT_Mu5'),
    HLTw3 = cms.untracked.string('HLT_Mu7'),
    HLTw4 = cms.untracked.string('HLT_Mu9'),
    HLTw5 = cms.untracked.string('HLT_Mu11'),
    HLTw6 = cms.untracked.string('HLT_Mu13'),
    HLTw7 = cms.untracked.string('HLT_Mu15'),
    HLTfiltObj2 = cms.untracked.string('hltL2MuonCandidates::HLT'),
    HLTfiltObj3 = cms.untracked.string('hltL3MuonCandidates::HLT')
   
)

TriggerDump = cms.Sequence(triggerDump)





 
