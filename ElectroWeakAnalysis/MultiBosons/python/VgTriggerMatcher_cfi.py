import FWCore.ParameterSet.Config as cms

# matches to HLT_Mu9
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

# matches to HLT_Mu30_v*
muonTriggerMatchHLTMu30 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu30_v*", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu24_v*
muonTriggerMatchHLTIsoMu24 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu24_v*", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu30_v*
muonTriggerMatchHLTIsoMu30 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu30_v*", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_DoubleMu6_v*
muonTriggerMatchHLTDoubleMu6 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_DoubleMu6_v*", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_DoubleMu7_v*
muonTriggerMatchHLTDoubleMu7 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_DoubleMu7_v*", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu13_Mu8_v*
muonTriggerMatchHLTMu13Mu8 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu13_Mu8_v*", 0, 1 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu17_Mu8_v*
muonTriggerMatchHLTMu17Mu8 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu17_Mu8_v*", 0, 1 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu24_eta2p1_v*
muonTriggerMatchHLTIsoMu24eta2p1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu24_eta2p1_v*", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu30_eta2p1_v*
muonTriggerMatchHLTIsoMu30eta2p1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu30_eta2p1_v*", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_IsoMu34_eta2p1_v*
muonTriggerMatchHLTIsoMu34eta2p1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_IsoMu34_eta2p1_v*", 1, 0 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Ele8_CaloIdL_CaloIsoVL_v*
electronTriggerMatchHLTEle8CaloIdLCaloIsoVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
						  matchedCuts = cms.string( 'filter("hltEle17CaloIdIsoEle8CaloIdIsoPixelMatchDoubleFilter")' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele17_CaloIdL_CaloIsoVL_v*
electronTriggerMatchHLTEle17CaloIdLCaloIsoVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
						  matchedCuts = cms.string( 'filter("hltEle17CaloIdLCaloIsoVLPixelMatchFilter")' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*
electronTriggerMatchHLTEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
						  matchedCuts = cms.string( 'filter("hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter")' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*
electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
                                                  matchedCuts = cms.string( 'filter("hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolFilter")' ),
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

# matches to HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v*
electronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoT    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v*
electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoT    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele27_WP80_PFMT50_v*
electronTriggerMatchHLTEle27WP80PFMT50    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele27_WP80_PFMT50_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele32_WP70_PFMT50_v*
electronTriggerMatchHLTEle32WP70PFMT50    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele32_WP70_PFMT50_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v*
electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVL    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v*
electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8Mass30    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v*
electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8Mass30 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*
electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_Ele17_v*
electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTEle17    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_Ele17_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_v*
electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele32_WP70_v*
electronTriggerMatchHLTEle32WP70    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle32WP70TrackIsoFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT*
electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVT = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsolFilter") || filter("hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele8_Mass30*
electronTriggerMatchHLTEle8Mass30 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8PMMassFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_SC8_Mass30*
electronTriggerMatchHLTSC8Mass30 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT*
electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoT = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17TrackIsolFilter") || filter("hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTEle17TrackIsolFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_SC17*
electronTriggerMatchHLTSC17 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17HEDoubleFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele17*
electronTriggerMatchHLTEle17 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTEle17PixelMatchDoubleFilter")' ),
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
							 matchedCuts = cms.string( 'path( "HLT_DoublePhoton17_L1R", 0, 1 )' ),
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

# matches HLT_Photon20_CaloIdVL_IsoL_v*
photonTriggerMatchHLTPhoton20CaloIdVLIsoL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Photon20_CaloIdVL_IsoL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matched HLT_Photon20_R9Id_Photon18_R9Id_v*
photonTriggerMatchHLTPhoton20R9IdPhoton18R9Id = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon20_R9Id_Photon18_R9Id_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon26_Photon18_v*
photonTriggerMatchHLTPhoton26Photon18 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_Photon18_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matched HLT_Photon26_IsoVL_Photon18_v*
photonTriggerMatchHLTPhoton26IsoVLPhoton18 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_IsoVL_Photon18_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon26_IsoVL_Photon18_IsoVL_v*
photonTriggerMatchHLTPhoton26IsoVLPhoton18IsoVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_IsoVL_Photon18_IsoVL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon26_CaloIdL_IsoVL_Photon18_v*
photonTriggerMatchHLTPhoton26CaloIdLIsoVLPhoton18 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_CaloIdL_IsoVL_Photon18_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL_v*
photonTriggerMatchHLTPhoton26CaloIdLIsoVLPhoton18CaloIdLIsoVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon26_CaloIdL_IsoVL_Photon18_R9Id_v*
photonTriggerMatchHLTPhoton26CaloIdLIsoVLPhoton18R9Id = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_CaloIdL_IsoVL_Photon18_R9Id_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon26_R9Id_Photon18_CaloIdL_IsoVL_v*
photonTriggerMatchHLTPhoton26R9IdPhoton18CaloIdLIsoVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_R9Id_Photon18_CaloIdL_IsoVL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon26_R9Id_Photon18_R9Id_v*
photonTriggerMatchHLTPhoton26R9IdPhoton18R9Id = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_R9Id_Photon18_R9Id_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon26_R9Id_Photon18_CaloIdXL_IsoXL_v*
photonTriggerMatchHLTPhoton26R9IdPhoton18CaloIdXLIsoXL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_R9Id_Photon18_CaloIdXL_IsoXL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon26_CaloIdXL_IsoXL_Photon18_v*
photonTriggerMatchHLTPhoton26CaloIdXLIsoXLPhoton18 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_CaloIdXL_IsoXL_Photon18_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon26_CaloIdXL_IsoXL_Photon18_R9Id_v*
photonTriggerMatchHLTPhoton26CaloIdXLIsoXLPhoton18R9Id = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_CaloIdXL_IsoXL_Photon18_R9Id_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon26_CaloIdXL_IsoXL_Photon18_CaloIdXL_IsoXL_v*
photonTriggerMatchHLTPhoton26CaloIdXLIsoXLPhoton18CaloIdXLIsoXL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_CaloIdXL_IsoXL_Photon18_CaloIdXL_IsoXL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon30_CaloIdVL_v*
photonTriggerMatchHLTPhoton30CaloIdVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon30_CaloIdVL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon30_CaloIdVL_IsoL_v*
photonTriggerMatchHLTPhoton30CaloIdVLIsoL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon30_CaloIdVL_IsoL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon32_CaloIdL_Photon26_CaloIdL_v*
photonTriggerMatchHLTPhoton32CaloIdLPhoton26CaloIdL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon32_CaloIdL_Photon26_CaloIdL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon36_CaloIdL_Photon22_CaloIdL_v*
photonTriggerMatchHLTPhoton36CaloIdLPhoton22CaloIdL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon36_CaloIdL_Photon22_CaloIdL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon36_CaloIdL_IsoVL_Photon22_v*
photonTriggerMatchHLTPhoton36CaloIdLIsoVLPhoton22 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon36_CaloIdL_IsoVL_Photon22_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon36_Photon22_v*
photonTriggerMatchHLTPhoton36Photon22 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon36_Photon22_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon36_IsoVL_Photon22_v*
photonTriggerMatchHLTPhoton36IsoVLPhoton22 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon36_IsoVL_Photon22_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon36_CaloId_IsoVL_Photon22_R9Id_v*
photonTriggerMatchHLTPhoton36CaloIdIsoVLPhoton22R9Id = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon36_CaloId_IsoVL_Photon22_R9Id_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon36_R9Id_Photon22_CaloIdL_IsoVL_v*
photonTriggerMatchHLTPhoton36R9IdPhoton22CaloIdLIsoVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon36_R9Id_Photon22_CaloIdL_IsoVL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon36_R9Id_Photon22_R9Id_v*
photonTriggerMatchHLTPhoton36R9IdPhoton22R9Id = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon36_R9Id_Photon22_R9Id_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon36_CaloIdL_IsoVL_Photon22_R9Id_v*
photonTriggerMatchHLTPhoton36CaloIdLIsoVLPhoton22R9Id = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon36_CaloIdL_IsoVL_Photon22_R9Id_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon36_CaloIdVL_Photon22_CaloIdVL_v*
photonTriggerMatchHLTPhoton36CaloIdVLPhoton22CaloIdVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon36_CaloIdVL_Photon22_CaloIdVL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon40_CaloIdL_Photon28_CaloIdL_v*
photonTriggerMatchHLTPhoton40CaloIdLPhoton28CaloIdL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon40_CaloIdL_Photon28_CaloIdL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon44_CaloIdL_Photon34_CaloIdL_v*
photonTriggerMatchHLTPhoton44CaloIdLPhoton34CaloIdL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon44_CaloIdL_Photon34_CaloIdL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon48_CaloIdL_Photon38_CaloIdL_v*
photonTriggerMatchHLTPhoton48CaloIdLPhoton38CaloIdL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon48_CaloIdL_Photon38_CaloIdL_v*", 0, 1 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon50_CaloIdVL_v*
photonTriggerMatchHLTPhoton50CaloIdVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon50_CaloIdVL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon50_CaloIdVL_IsoL_v*
photonTriggerMatchHLTPhoton50CaloIdVLIsoL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon50_CaloIdVL_IsoL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon75_CaloIdVL_v*
photonTriggerMatchHLTPhoton75CaloIdVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon75_CaloIdVL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon75_CaloIdVL_IsoL_v*
photonTriggerMatchHLTPhoton75CaloIdVLIsoL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon75_CaloIdVL_IsoL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon90_CaloIdVL_v*
photonTriggerMatchHLTPhoton90CaloIdVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon90_CaloIdVL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches HLT_Photon90_CaloIdVL_IsoL_v*
photonTriggerMatchHLTPhoton90CaloIdVLIsoL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon90_CaloIdVL_IsoL_v*", 1, 0 )' ),
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

# matches to HLT_Jet30_v*
jetTriggerMatchHLTJet30   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet30_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet60_v*
jetTriggerMatchHLTJet60   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet60_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet80_v*
jetTriggerMatchHLTJet80   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet80_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet110_v*
jetTriggerMatchHLTJet110   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet110_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet150_v*
jetTriggerMatchHLTJet150   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet150_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet190_v*
jetTriggerMatchHLTJet190   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet190_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet240_v*
jetTriggerMatchHLTJet240   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet240_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet300_v*
jetTriggerMatchHLTJet300   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet300_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet370_v*
jetTriggerMatchHLTJet370   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Jet370_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

vgTriggerMatcherElectron = cms.Sequence(
        electronTriggerMatchHLTEle15LWL1R +
        electronTriggerMatchHLTEle15SWL1R +
        electronTriggerMatchHLTEle15SWCaloEleIdL1R +
        electronTriggerMatchHLTEle17SWCaloEleIdL1R +
        electronTriggerMatchHLTEle17SWTightEleIdL1R +
	electronTriggerMatchHLTEle17SWTighterEleIdIsolL1R +
	electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv2 +
	electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv3 +
	electronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoT + 
        electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoT +
	electronTriggerMatchHLTEle27WP80PFMT50 +
	electronTriggerMatchHLTEle32WP70PFMT50 +
        electronTriggerMatchHLTEle8CaloIdLCaloIsoVL +
        electronTriggerMatchHLTEle17CaloIdLCaloIsoVL +
        electronTriggerMatchHLTEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL + 
	electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVL +
	electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVL +
        electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8Mass30 + 
        electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8Mass30 + 
	electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL +
        electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTEle17 +
        electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17 +
	electronTriggerMatchHLTEle32WP70 +
	electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVT +
	electronTriggerMatchHLTEle8Mass30 +
	electronTriggerMatchHLTSC8Mass30 +
	electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoT +
	electronTriggerMatchHLTSC17 +
	electronTriggerMatchHLTEle17
        )

vgTriggerMatcherPhoton = cms.Sequence(
	photonTriggerMatchHLTPhoton10CleanedL1R +
	photonTriggerMatchHLTPhoton15CleanedL1R +
	photonTriggerMatchHLTPhoton20CleanedL1R +
	photonTriggerMatchHLTPhoton30CleanedL1R +
	photonTriggerMatchHLTPhoton50CleanedL1Rv1 +
	photonTriggerMatchHLTPhoton70CleanedL1Rv1 +
        photonTriggerMatchHLTDoublePhoton17L1R +
	photonTriggerMatchHLTPhoton10L1R +
	photonTriggerMatchHLTPhoton20CaloIdVLIsoL +
	photonTriggerMatchHLTPhoton20R9IdPhoton18R9Id +
	photonTriggerMatchHLTPhoton26Photon18 +
	photonTriggerMatchHLTPhoton26IsoVLPhoton18 +
	photonTriggerMatchHLTPhoton26IsoVLPhoton18IsoVL +
	photonTriggerMatchHLTPhoton26CaloIdLIsoVLPhoton18 +
	photonTriggerMatchHLTPhoton26CaloIdLIsoVLPhoton18CaloIdLIsoVL +
	photonTriggerMatchHLTPhoton26CaloIdLIsoVLPhoton18R9Id +
	photonTriggerMatchHLTPhoton26R9IdPhoton18CaloIdLIsoVL +
	photonTriggerMatchHLTPhoton26R9IdPhoton18R9Id +
	photonTriggerMatchHLTPhoton26R9IdPhoton18CaloIdXLIsoXL +
	photonTriggerMatchHLTPhoton26CaloIdXLIsoXLPhoton18 +
	photonTriggerMatchHLTPhoton26CaloIdXLIsoXLPhoton18R9Id +
	photonTriggerMatchHLTPhoton26CaloIdXLIsoXLPhoton18CaloIdXLIsoXL +
	photonTriggerMatchHLTPhoton30CaloIdVL +
	photonTriggerMatchHLTPhoton30CaloIdVLIsoL + 
	photonTriggerMatchHLTPhoton32CaloIdLPhoton26CaloIdL +
	photonTriggerMatchHLTPhoton36CaloIdLPhoton22CaloIdL +
	photonTriggerMatchHLTPhoton36CaloIdLIsoVLPhoton22 +
	photonTriggerMatchHLTPhoton36Photon22 +
	photonTriggerMatchHLTPhoton36IsoVLPhoton22 +
	photonTriggerMatchHLTPhoton36CaloIdIsoVLPhoton22R9Id +
	photonTriggerMatchHLTPhoton36R9IdPhoton22CaloIdLIsoVL +
	photonTriggerMatchHLTPhoton36R9IdPhoton22R9Id +
	photonTriggerMatchHLTPhoton36CaloIdLIsoVLPhoton22R9Id +
	photonTriggerMatchHLTPhoton36CaloIdVLPhoton22CaloIdVL +
	photonTriggerMatchHLTPhoton40CaloIdLPhoton28CaloIdL +
	photonTriggerMatchHLTPhoton44CaloIdLPhoton34CaloIdL +
	photonTriggerMatchHLTPhoton48CaloIdLPhoton38CaloIdL +
	photonTriggerMatchHLTPhoton50CaloIdVL +
	photonTriggerMatchHLTPhoton50CaloIdVLIsoL +
	photonTriggerMatchHLTPhoton75CaloIdVL +
	photonTriggerMatchHLTPhoton75CaloIdVLIsoL +
	photonTriggerMatchHLTPhoton90CaloIdVL +
	photonTriggerMatchHLTPhoton90CaloIdVLIsoL
	)

vgTriggerMatcherMuon = cms.Sequence(
    muonTriggerMatchHLTMu9 +
    muonTriggerMatchHLTMu11 +
    muonTriggerMatchHLTMu13 +
    muonTriggerMatchHLTMu13v1 +
    muonTriggerMatchHLTMu15 +
    muonTriggerMatchHLTMu15v1 +
    muonTriggerMatchHLTMu30 +
    muonTriggerMatchHLTIsoMu24 +
    muonTriggerMatchHLTIsoMu30 +
    muonTriggerMatchHLTIsoMu24eta2p1 +
    muonTriggerMatchHLTIsoMu30eta2p1 +
    muonTriggerMatchHLTIsoMu34eta2p1 +
    muonTriggerMatchHLTDoubleMu6 +
    muonTriggerMatchHLTDoubleMu7 +
    muonTriggerMatchHLTMu13Mu8 + 
    muonTriggerMatchHLTMu17Mu8
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
	jetTriggerMatchHLTJet30 +
	jetTriggerMatchHLTJet60 +
	jetTriggerMatchHLTJet80  +
	jetTriggerMatchHLTJet110 +
	jetTriggerMatchHLTJet150 +
	jetTriggerMatchHLTJet190 +
	jetTriggerMatchHLTJet240 +
        jetTriggerMatchHLTJet300 +
	jetTriggerMatchHLTJet370
	)

vgTriggerMatcher = cms.Sequence(
    vgTriggerMatcherElectron + vgTriggerMatcherMuon + vgTriggerMatcherPhoton + vgTriggerMatcherJet
    )
