import FWCore.ParameterSet.Config as cms

L2TauTriggerEfficiency = cms.EDAnalyzer("L2TauEfficiencyAnalyzer",
                                                 PFTauCollection = cms.InputTag("pfRecoTauProducer"),

                                                 L2AssociationCollection = cms.InputTag("hltL2DoubleTauIsolationProducer","L2TauIsolationInfoAssociator"),
                                                 outputFileName = cms.string("test.root"),
                                                 matchingDeltaR =cms.double(0.3)
                                                )
