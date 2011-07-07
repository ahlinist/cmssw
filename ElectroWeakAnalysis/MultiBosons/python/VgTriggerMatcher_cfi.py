import FWCore.ParameterSet.Config as cms

# matches to HLT_Mu9
#muonTriggerMatchHLTMu9 = cms.EDProducer("PATTriggerMatcherDRDPtLessByR",
muonTriggerMatchHLTMu9 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
				      matchedCuts = cms.string( 'path( "HLT_Mu9", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu11
muonTriggerMatchHLTMu11 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
				      matchedCuts = cms.string( 'path( "HLT_Mu11", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu13
muonTriggerMatchHLTMu13 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
				      matchedCuts = cms.string( 'path( "HLT_Mu13", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu13_v1
muonTriggerMatchHLTMu13v1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
				      matchedCuts = cms.string( 'path( "HLT_Mu13_v1", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu15
muonTriggerMatchHLTMu15 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
				      matchedCuts = cms.string( 'path( "HLT_Mu15", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu15_v1
muonTriggerMatchHLTMu15v1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
				      matchedCuts = cms.string( 'path( "HLT_Mu15_v1", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu24_v1
muonTriggerMatchHLTMu24v1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu24_v1", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu24_v2
muonTriggerMatchHLTMu24v2 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu24_v2", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu24_v3
muonTriggerMatchHLTMu24v3 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu24_v3", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu24_v5
muonTriggerMatchHLTMu24v5 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu24_v5", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu30_v1
muonTriggerMatchHLTMu30v1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu30_v1", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu30_v2
muonTriggerMatchHLTMu30v2 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu30_v2", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu30_v3
muonTriggerMatchHLTMu30v3 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu30_v3", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu30_v5
muonTriggerMatchHLTMu30v5 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu30_v5", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu40_v1
muonTriggerMatchHLTMu40v1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu40_v1", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu17_v4
muonTriggerMatchHLTIsoMu17v4 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu17_v4", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu17_v5
muonTriggerMatchHLTIsoMu17v5 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu17_v5", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu17_v6
muonTriggerMatchHLTIsoMu17v6 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu17_v6", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu17_v8
muonTriggerMatchHLTIsoMu17v8 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu17_v8", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu17_v9
muonTriggerMatchHLTIsoMu17v9 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu17_v9", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu17_v11
muonTriggerMatchHLTIsoMu17v11 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu17_v11", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu24_v1
muonTriggerMatchHLTIsoMu24v1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu24_v1", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu24_v2
muonTriggerMatchHLTIsoMu24v2 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu24_v2", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu24_v4
muonTriggerMatchHLTIsoMu24v4 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu24_v4", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu24_v5
muonTriggerMatchHLTIsoMu24v5 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu24_v5", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu24_v7
muonTriggerMatchHLTIsoMu24v7 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu24_v7", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu30_v1
muonTriggerMatchHLTIsoMu30v1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu30_v1", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu30_v2
muonTriggerMatchHLTIsoMu30v2 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu30_v2", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu30_v4
muonTriggerMatchHLTIsoMu30v4 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu30_v4", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu30_v5
muonTriggerMatchHLTIsoMu30v5 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu30_v5", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu30_v7
muonTriggerMatchHLTIsoMu30v7 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu30_v7", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_DoubleMu6_v1
muonTriggerMatchHLTDoubleMu6v1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_DoubleMu6_v1", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_DoubleMu6_v2
muonTriggerMatchHLTDoubleMu6v2 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_DoubleMu6_v2", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_DoubleMu6_v3
muonTriggerMatchHLTDoubleMu6v3 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_DoubleMu6_v3", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_DoubleMu7_v1
muonTriggerMatchHLTDoubleMu7v1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_DoubleMu7_v1", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_DoubleMu7_v2
muonTriggerMatchHLTDoubleMu7v2 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_DoubleMu7_v2", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )           

# matches to HLT_DoubleMu7_v3
muonTriggerMatchHLTDoubleMu7v3 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_DoubleMu7_v3", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )           

# matches to HLT_Mu13_Mu8_v2
muonTriggerMatchHLTMu13Mu8v2 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu13_Mu8_v2", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu13_Mu8_v4
muonTriggerMatchHLTMu13Mu8v4 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu13_Mu8_v4", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu17_Mu8_v2
muonTriggerMatchHLTMu17Mu8v2 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu17_Mu8_v2", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu17_Mu8_v4
muonTriggerMatchHLTMu17Mu8v4 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu17_Mu8_v4", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Ele8_CaloIdL_CaloIsoVL_v*
electronTriggerMatchHLTEle8CaloIdLCaloIsoVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
                                                  matchedCuts = cms.string( 'path( "HLT_Ele8_CaloIdL_CaloIsoVL_v*", 1, 0 )' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele17_CaloIdL_CaloIsoVL_v*
electronTriggerMatchHLTEle17CaloIdLCaloIsoVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
                                                  matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdL_CaloIsoVL_v*", 1, 0 )' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele15_LW_L1R
electronTriggerMatchHLTEle15LWL1R = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
						  matchedCuts = cms.string( 'path( "HLT_Ele15_LW_L1R", 1, 0 )' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele15_SW_L1R
electronTriggerMatchHLTEle15SWL1R = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
						  matchedCuts = cms.string( 'path( "HLT_Ele15_SW_L1R", 1, 0 )' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele20_SW_L1R
electronTriggerMatchHLTEle20SWL1R = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
						  matchedCuts = cms.string( 'path( "HLT_Ele20_SW_L1R", 1, 0 )' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele15_SW_EleId_L1R
electronTriggerMatchHLTEle15SWEleIdL1R = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
						  matchedCuts = cms.string( 'path( "HLT_Ele15_SW_EleId_L1R", 1, 0 )' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele15_SW_CaloEleId_L1R
electronTriggerMatchHLTEle15SWCaloEleIdL1R = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Ele15_SW_CaloEleId_L1R", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_CaloEleId_L1R 
electronTriggerMatchHLTEle17SWCaloEleIdL1R  = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Ele17_SW_CaloEleId_L1R", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_TightEleId_L1R 
electronTriggerMatchHLTEle17SWTightEleIdL1R   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Ele17_SW_TightEleId_L1R", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1
electronTriggerMatchHLTEle17SWTightCaloEleIdEle8HEL1Rv1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2
electronTriggerMatchHLTEle17SWTightCaloEleIdEle8HEL1Rv2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_TighterEleIdIsol_L1R 
electronTriggerMatchHLTEle17SWTighterEleIdIsolL1R    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Ele17_SW_TighterEleIdIsol_L1R", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_TighterEleIdIsol_L1R_v2
electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv2    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Ele17_SW_TighterEleIdIsol_L1R_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_TighterEleIdIsol_L1R_v3
electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv3    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Ele17_SW_TighterEleIdIsol_L1R_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele22_SW_TighterEleId_L1R_v2
electronTriggerMatchHLTEle22SWTighterEleIdL1Rv2    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Ele22_SW_TighterEleId_L1R_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1
electronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTv1    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2
electronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTv2    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3
electronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTv3    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1
electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTv1    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2
electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTv2    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3
electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTv3    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v4
electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTv4    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v4", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v5
electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTv5    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v5", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele42_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1
electronTriggerMatchHLTEle42CaloIdVTCaloIsoTTrkIdTTrkIsoTv1    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele42_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele25_WP80_PFMT40_v1
electronTriggerMatchHLTEle25WP80PFMT40v1    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele25_WP80_PFMT40_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele27_WP80_PFMT50_v1
electronTriggerMatchHLTEle27WP80PFMT50v1    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele27_WP80_PFMT50_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele32_WP70_PFMT50_v1
electronTriggerMatchHLTEle32WP70PFMT50v1    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele32_WP70_PFMT50_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele27_WP70_PFMT40_PFMHT20_v1
electronTriggerMatchHLTEle27WP70PFMT40PFMHT20v1    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele27_WP70_PFMT40_PFMHT20_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1
electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVLv1    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2
electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVLv2    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v3
electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVLv3    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v4
electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVLv4    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v4", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v5
electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVLv5    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v5", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v6
electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVLv6    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v6", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Photon10_Cleaned_L1R
photonTriggerMatchHLTPhoton10CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Photon10_Cleaned_L1R", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Photon15_Cleaned_L1R
photonTriggerMatchHLTPhoton15CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Photon15_Cleaned_L1R", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Photon20_Cleaned_L1R
photonTriggerMatchHLTPhoton20CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Photon20_Cleaned_L1R", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Photon30_Cleaned_L1R
photonTriggerMatchHLTPhoton30CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Photon30_Cleaned_L1R", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Photon50_Cleaned_L1R_v1
photonTriggerMatchHLTPhoton50CleanedL1Rv1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Photon50_Cleaned_L1R_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Photon70_Cleaned_L1R_v1
photonTriggerMatchHLTPhoton70CleanedL1Rv1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Photon70_Cleaned_L1R_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_DoublePhoton17_L1R
photonTriggerMatchHLTDoublePhoton17L1R = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                         src     = cms.InputTag( "cleanPatPhotons" ),
                                                         matched = cms.InputTag( "patTrigger" ),
							 matchedCuts = cms.string( 'path( "HLT_DoublePhoton17_L1R", 1, 0 )' ),
                                                         maxDPtRel = cms.double( 0.5 ),
                                                         maxDeltaR = cms.double( 0.3 ),
                                                         resolveAmbiguities    = cms.bool( True ),
                                                         resolveByMatchQuality = cms.bool( True )
                                                         )

# matches to HLT_Photon10_L1R
photonTriggerMatchHLTPhoton10L1R   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Photon10_L1R", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet15U
jetTriggerMatchHLTJet15U   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet15U", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet30U
jetTriggerMatchHLTJet30U   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet30U", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet50U
jetTriggerMatchHLTJet50U   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet50U", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet70U
jetTriggerMatchHLTJet70U   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet70U", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet70U_v2
jetTriggerMatchHLTJet70Uv2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet70U_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet100U
jetTriggerMatchHLTJet100U   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet100U", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet100U_v2
jetTriggerMatchHLTJet100Uv2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet100U_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet140U_v1
jetTriggerMatchHLTJet140Uv1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet140U_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet15U_v3
jetTriggerMatchHLTJet15Uv3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet15U_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet30U_v3
jetTriggerMatchHLTJet30Uv3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet30U_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet50U_v3
jetTriggerMatchHLTJet50Uv3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet50U_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet70U_v3
jetTriggerMatchHLTJet70Uv3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet70U_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet100U_v3
jetTriggerMatchHLTJet100Uv3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet100U_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet140U_v3
jetTriggerMatchHLTJet140Uv3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Jet140U_v3", 1, 0 )' ),
                                                           #andOr          = cms.bool( False ),
                                                           #filterIdsEnum  = cms.vstring( '*' ),
                                                           #filterIds      = cms.vint32( 0 ),
                                                           #filterLabels   = cms.vstring( '*' ),
                                                           #pathNames      = cms.vstring( 'HLT_Jet140U_v3' ),
                                                           #collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet30_v1
jetTriggerMatchHLTJet30v1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet30_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet30_v2
jetTriggerMatchHLTJet30v2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet30_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet30_v3
jetTriggerMatchHLTJet30v3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet30_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet30_v4
jetTriggerMatchHLTJet30v4   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet30_v4", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet30_v6
jetTriggerMatchHLTJet30v6   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet30_v6", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet60_v1
jetTriggerMatchHLTJet60v1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet60_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet60_v2
jetTriggerMatchHLTJet60v2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet60_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet60_v3
jetTriggerMatchHLTJet60v3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet60_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet60_v4
jetTriggerMatchHLTJet60v4   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet60_v4", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet60_v6
jetTriggerMatchHLTJet60v6   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet60_v6", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet80_v1
jetTriggerMatchHLTJet80v1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet80_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet80_v2
jetTriggerMatchHLTJet80v2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet80_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet80_v3
jetTriggerMatchHLTJet80v3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet80_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet80_v4
jetTriggerMatchHLTJet80v4   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet80_v4", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet80_v6
jetTriggerMatchHLTJet80v6   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet80_v6", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet110_v1
jetTriggerMatchHLTJet110v1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet110_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet110_v2
jetTriggerMatchHLTJet110v2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet110_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet110_v3
jetTriggerMatchHLTJet110v3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet110_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet110_v4
jetTriggerMatchHLTJet110v4   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet110_v4", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet110_v6
jetTriggerMatchHLTJet110v6   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet110_v6", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet150_v1
jetTriggerMatchHLTJet150v1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet150_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet150_v2
jetTriggerMatchHLTJet150v2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet150_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet150_v3
jetTriggerMatchHLTJet150v3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet150_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet150_v4
jetTriggerMatchHLTJet150v4   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet150_v4", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet150_v6
jetTriggerMatchHLTJet150v6   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet150_v6", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet190_v1
jetTriggerMatchHLTJet190v1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet190_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet190_v2
jetTriggerMatchHLTJet190v2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet190_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet190_v3
jetTriggerMatchHLTJet190v3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet190_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet190_v4
jetTriggerMatchHLTJet190v4   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet190_v4", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet190_v6
jetTriggerMatchHLTJet190v6   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet190_v6", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet240_v1
jetTriggerMatchHLTJet240v1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet240_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet240_v2
jetTriggerMatchHLTJet240v2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet240_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet240_v3
jetTriggerMatchHLTJet240v3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet240_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet240_v4
jetTriggerMatchHLTJet240v4   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet240_v4", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet240_v6
jetTriggerMatchHLTJet240v6   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet240_v6", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet300_v1
jetTriggerMatchHLTJet300v1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet300_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet300_v2
jetTriggerMatchHLTJet300v2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet300_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet300_v3
jetTriggerMatchHLTJet300v3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet300_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet300_v5
jetTriggerMatchHLTJet300v5   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet300_v5", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet370_v1
jetTriggerMatchHLTJet370v1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet370_v1", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet370_v2
jetTriggerMatchHLTJet370v2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet370_v2", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet370_v3
jetTriggerMatchHLTJet370v3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet370_v3", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet370_v4
jetTriggerMatchHLTJet370v4   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet370_v4", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet370_v6
jetTriggerMatchHLTJet370v6   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet370_v6", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

vgTriggerMatcherElectron = cms.Sequence(
        electronTriggerMatchHLTEle8CaloIdLCaloIsoVL +
        electronTriggerMatchHLTEle17CaloIdLCaloIsoVL +
        electronTriggerMatchHLTEle15LWL1R +
        electronTriggerMatchHLTEle15SWL1R +
        electronTriggerMatchHLTEle20SWL1R +
        electronTriggerMatchHLTEle15SWEleIdL1R +
        electronTriggerMatchHLTEle15SWCaloEleIdL1R +
        electronTriggerMatchHLTEle17SWCaloEleIdL1R +
        electronTriggerMatchHLTEle17SWTightEleIdL1R +
	electronTriggerMatchHLTEle17SWTighterEleIdIsolL1R +
	electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv2 +
	electronTriggerMatchHLTEle22SWTighterEleIdL1Rv2 +
	electronTriggerMatchHLTEle17SWTightCaloEleIdEle8HEL1Rv1 +
	electronTriggerMatchHLTEle17SWTightCaloEleIdEle8HEL1Rv2 +
	electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv3 +
	electronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTv1 + 
	electronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTv2 + 
	electronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTv3 + 
        electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTv1 +
        electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTv2 +
        electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTv3 +
        electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTv4 +
        electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTv5 +
        electronTriggerMatchHLTEle42CaloIdVTCaloIsoTTrkIdTTrkIsoTv1 +
	electronTriggerMatchHLTEle25WP80PFMT40v1 +
	electronTriggerMatchHLTEle27WP80PFMT50v1 +
	electronTriggerMatchHLTEle32WP70PFMT50v1 +
	electronTriggerMatchHLTEle27WP70PFMT40PFMHT20v1 +
	electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVLv1 +
	electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVLv2 +
	electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVLv3 +
	electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVLv4 +
	electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVLv5 +
	electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVLv6
        )

vgTriggerMatcherPhoton = cms.Sequence(
	photonTriggerMatchHLTPhoton10CleanedL1R +
	photonTriggerMatchHLTPhoton15CleanedL1R +
	photonTriggerMatchHLTPhoton20CleanedL1R +
	photonTriggerMatchHLTPhoton30CleanedL1R +
	photonTriggerMatchHLTPhoton50CleanedL1Rv1 +
	photonTriggerMatchHLTPhoton70CleanedL1Rv1 +
        photonTriggerMatchHLTDoublePhoton17L1R +
	photonTriggerMatchHLTPhoton10L1R
	)

vgTriggerMatcherMuon = cms.Sequence(
    muonTriggerMatchHLTMu9 +
    muonTriggerMatchHLTMu11 +
    muonTriggerMatchHLTMu13 +
    muonTriggerMatchHLTMu13v1 +
    muonTriggerMatchHLTMu15 +
    muonTriggerMatchHLTMu15v1 +
    muonTriggerMatchHLTMu24v1 +
    muonTriggerMatchHLTMu24v2 +
    muonTriggerMatchHLTMu24v3 +
    muonTriggerMatchHLTMu30v1 +
    muonTriggerMatchHLTMu30v2 +
    muonTriggerMatchHLTMu30v3 +
    muonTriggerMatchHLTIsoMu17v4 +
    muonTriggerMatchHLTIsoMu17v5 +
    muonTriggerMatchHLTIsoMu17v6 +
    muonTriggerMatchHLTIsoMu17v8 +
    muonTriggerMatchHLTIsoMu17v9 +
    muonTriggerMatchHLTIsoMu24v1 +
    muonTriggerMatchHLTIsoMu24v2 +
    muonTriggerMatchHLTIsoMu24v4 +
    muonTriggerMatchHLTIsoMu24v5 +
    muonTriggerMatchHLTIsoMu30v1 +
    muonTriggerMatchHLTIsoMu30v2 +
    muonTriggerMatchHLTIsoMu30v4 +
    muonTriggerMatchHLTIsoMu30v5 +
    muonTriggerMatchHLTDoubleMu6v1 +
    muonTriggerMatchHLTDoubleMu6v2 +
    muonTriggerMatchHLTDoubleMu6v3 +
    muonTriggerMatchHLTDoubleMu7v1 + 
    muonTriggerMatchHLTDoubleMu7v2 + 
    muonTriggerMatchHLTDoubleMu7v3 + 
    muonTriggerMatchHLTMu13Mu8v2 +
    muonTriggerMatchHLTMu24v5 +
    muonTriggerMatchHLTMu30v5 +
    muonTriggerMatchHLTIsoMu17v11 +
    muonTriggerMatchHLTIsoMu24v7 +
    muonTriggerMatchHLTIsoMu30v7 +
    muonTriggerMatchHLTMu13Mu8v4 +
    muonTriggerMatchHLTMu17Mu8v2 +
    muonTriggerMatchHLTMu17Mu8v4
    )

vgTriggerMatcherJet = cms.Sequence(
	jetTriggerMatchHLTJet15U +
	jetTriggerMatchHLTJet30U +
	jetTriggerMatchHLTJet50U +
	jetTriggerMatchHLTJet70U +
	jetTriggerMatchHLTJet70Uv2 +
	jetTriggerMatchHLTJet100U +
	jetTriggerMatchHLTJet100Uv2 +
	jetTriggerMatchHLTJet140Uv1 +
	jetTriggerMatchHLTJet15Uv3 +
	jetTriggerMatchHLTJet30Uv3 +
	jetTriggerMatchHLTJet50Uv3 +
	jetTriggerMatchHLTJet70Uv3 +
	jetTriggerMatchHLTJet100Uv3 +
	jetTriggerMatchHLTJet140Uv3 +
	jetTriggerMatchHLTJet30v1 +
	jetTriggerMatchHLTJet30v2 +
	jetTriggerMatchHLTJet30v3 +
	jetTriggerMatchHLTJet30v4 +
	jetTriggerMatchHLTJet60v1 +
	jetTriggerMatchHLTJet60v2 +
	jetTriggerMatchHLTJet60v3 +
	jetTriggerMatchHLTJet60v4 +
	jetTriggerMatchHLTJet80v1 +
	jetTriggerMatchHLTJet80v2 +
	jetTriggerMatchHLTJet80v3 +
	jetTriggerMatchHLTJet80v4 +
	jetTriggerMatchHLTJet110v1 +
	jetTriggerMatchHLTJet110v2 +
	jetTriggerMatchHLTJet110v3 +
	jetTriggerMatchHLTJet110v4 +
	jetTriggerMatchHLTJet150v1 +
	jetTriggerMatchHLTJet150v2 +
	jetTriggerMatchHLTJet150v3 +
	jetTriggerMatchHLTJet150v4 +
	jetTriggerMatchHLTJet190v1 +
	jetTriggerMatchHLTJet190v2 +
	jetTriggerMatchHLTJet190v3 +
	jetTriggerMatchHLTJet190v4 +
	jetTriggerMatchHLTJet240v1 +
	jetTriggerMatchHLTJet240v2 +
	jetTriggerMatchHLTJet240v3 +
	jetTriggerMatchHLTJet240v4 +
        jetTriggerMatchHLTJet300v1 +
        jetTriggerMatchHLTJet300v2 +
        jetTriggerMatchHLTJet300v3 +
	jetTriggerMatchHLTJet370v1 +
	jetTriggerMatchHLTJet370v2 +
	jetTriggerMatchHLTJet370v3 +
	jetTriggerMatchHLTJet370v4 +
	jetTriggerMatchHLTJet30v6 +
	jetTriggerMatchHLTJet60v6 +
	jetTriggerMatchHLTJet80v6  +
	jetTriggerMatchHLTJet110v6 +
	jetTriggerMatchHLTJet150v6 +
	jetTriggerMatchHLTJet190v6 +
	jetTriggerMatchHLTJet240v6 +
        jetTriggerMatchHLTJet300v5 +
	jetTriggerMatchHLTJet370v6
	)

vgTriggerMatcher = cms.Sequence(
    vgTriggerMatcherElectron + vgTriggerMatcherMuon + vgTriggerMatcherPhoton + vgTriggerMatcherJet
    )
