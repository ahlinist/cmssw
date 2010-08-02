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
    hltLabel = cms.InputTag("TriggerResults","","HLT"),
    #hltLabel = cms.InputTag("TriggerResults","","HLT8E29"),
    hltobjectLabel = cms.InputTag("hltTriggerSummaryAOD"), 
    #hltobjectLabel = cms.InputTag("hltTriggerSummaryAOD","","HLT8E29"),                       
    HLTriggerName = cms.untracked.string('HLT_Mu9'),
    HLTw1 = cms.untracked.string('HLT_Mu3'),
    HLTw2 = cms.untracked.string('HLT_Mu5'),
    HLTw3 = cms.untracked.string('HLT_Mu9'),
    HLTw4 = cms.untracked.string('HLT_L1Mu'),
    HLTw5 = cms.untracked.string('HLT_L1MuOpen'),
    HLTw6 = cms.untracked.string('HLT_DoubleMu0'),
    HLTw7 = cms.untracked.string('HLT_L1_BscMinBiasOR_BptxPlusORMinus'),
    #HLTfiltObj2 = cms.untracked.string('hltL2MuonCandidates::HLT8E29'),
    #HLTfiltObj3 = cms.untracked.string('hltL3MuonCandidates::HLT8E29'),
    HLTfiltObj2 = cms.untracked.string('hltL2MuonCandidates::HLT'),
    HLTfiltObj3 = cms.untracked.string('hltL3MuonCandidates::HLT'),
    fileName = cms.string('trigger.root')
   
)

TriggerDump = cms.Sequence(triggerDump)





 
