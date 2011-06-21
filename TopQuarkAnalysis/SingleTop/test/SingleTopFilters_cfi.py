import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi


HLTFilterMuOrEle = cms.EDFilter('SingleTopTriggers',
                           HLTriggerResults = cms.InputTag("TriggerResults","","HLT"),
                           isMC = cms.untracked.bool(False),
                           channel = cms.untracked.int32(3),
                           )                         

HLTFilterMu = cms.EDFilter('SingleTopTriggers',
                           HLTriggerResults = cms.InputTag("TriggerResults","","HLT"),
                           isMC = cms.untracked.bool(False),
                           channel = cms.untracked.int32(1),
                           )                         

HLTFilterEle = cms.EDFilter('SingleTopTriggers',
                            HLTriggerResults = cms.InputTag("TriggerResults","","HLT"),
                            isMC = cms.untracked.bool(False),
                            channel = cms.untracked.int32(2),
                            )

HLTFilterMuQCD = cms.EDFilter('SingleTopTriggers',
                           HLTriggerResults = cms.InputTag("TriggerResults","","HLT"),
                           isMC = cms.untracked.bool(False),
                           channel = cms.untracked.int32(4),
                           )                         

HLTFilterEleQCD = cms.EDFilter('SingleTopTriggers',
                            HLTriggerResults = cms.InputTag("TriggerResults","","HLT"),
                            isMC = cms.untracked.bool(False),
                            channel = cms.untracked.int32(5),
                            )

