import FWCore.ParameterSet.Config as cms

triggerDump = cms.EDAnalyzer("HFDumpTrigger",
    verbose = cms.untracked.int32(0),
    dol1 = cms.untracked.int32(1),
    dohlt = cms.untracked.int32(1),
    GlobalTriggerLabel = cms.untracked.string('gtDigis'),
    L1MuLabel = cms.untracked.string('hltL1extraParticles'),
    L1TriggerName = cms.untracked.string('L1_SingleMu0'),
    L1w1 = cms.untracked.string('L1_SingleMu3'),
    L1w2 = cms.untracked.string('L1_SingleMu5'),
    L1w3 = cms.untracked.string('L1_SingleMu7'),
    L1w4 = cms.untracked.string('L1_SingleMuOpen'),
    hltLabel = cms.InputTag("TriggerResults","","REDIGI37X"),
    hltobjectLabel = cms.InputTag("hltTriggerSummaryAOD","","REDIGI37X"),                     
    HLTriggerName = cms.untracked.string('HLT_Mu9'),
    HLTw1 = cms.untracked.string('HLT_Mu3'),
    HLTw2 = cms.untracked.string('HLT_Mu5'),
    HLTw3 = cms.untracked.string('HLT_Mu9'),
    HLTw4 = cms.untracked.string('HLT_L1Mu'),
    HLTw5 = cms.untracked.string('HLT_L1MuOpen'),
    HLTw6 = cms.untracked.string('HLT_DoubleMu0'),
    HLTw7 = cms.untracked.string('HLT_DoubleMu3'),
    HLTfiltObj2 = cms.untracked.string('hltL2MuonCandidates::REDIGI37X'),
    HLTfiltObj3 = cms.untracked.string('hltL3MuonCandidates::REDIGI37X'),
    fileName = cms.string('trigger.root')
   
)

TriggerDump = cms.Sequence(triggerDump)





 
