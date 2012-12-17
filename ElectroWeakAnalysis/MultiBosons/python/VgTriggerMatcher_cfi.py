import FWCore.ParameterSet.Config as cms

# matches to HLT_Mu17_Mu8_v*
muonTriggerMatchHLTMu17Mu8 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu17_Mu8_v*", 1, 0 ) || path( "HLT_Mu17_Mu8_v*", 0, 1 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu17_TkMu8_v*
muonTriggerMatchHLTMu17TkMu8 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'path( "HLT_Mu17_TkMu8_v*", 1, 0 ) || path( "HLT_Mu17_TkMu8_v*", 0, 1 )' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu17_v*
muonTriggerMatchHLTMu17forMu17Mu8 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'filter("hltL3fL1DoubleMu10MuOpenL1f0L2f10L3Filtered17") || filter("hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17")' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu17_v*
muonTriggerMatchHLTMu17forMu17TkMu8 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'filter("hltL3fL1sMu10MuOpenL1f0L2f10L3Filtered17") || filter("hltL3fL1sMu10MuOpenOR3p5L1f0L2f10L3Filtered17")' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu8_v*
muonTriggerMatchHLTMu8forMu17Mu8 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'filter("hltL3pfL1DoubleMu10MuOpenL1f0L2pf0L3PreFiltered8") || filter("hltL3pfL1DoubleMu10MuOpenOR3p5L1f0L2pf0L3PreFiltered8")' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_TkMu8_v*
muonTriggerMatchHLTTkMu8forMu17TkMu8 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      matchedCuts = cms.string( 'filter("hltDiMuonGlb17Trk8DzFiltered0p2") || filter("hltDiMuonGlb17Trk8DzFiltered0p2")' ),
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

# matches to HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*
electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*
electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
		                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoFilter") || filter("hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*
electronTriggerMatchHLTEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoDoubleFilter") || filter("hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass50_v*
electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8Mass50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass50_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v*
electronTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4Mass50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_Mass50_v*
electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17Mass50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_Mass50_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele27_WP80_PFMET_MT50_v*
electronTriggerMatchHLTEle27WP80PFMETMT50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Ele27_WP80_PFMET_MT50_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele27_WP80_v*
electronTriggerMatchHLTEle27WP80 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'path( "HLT_Ele27_WP80_v*", 1, 0 ) || filter("hltEle27WP80TrackIsoFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT*
electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVT = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsolFilter") || filter("hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsoFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT
electronTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVT = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'filter("hltEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4TrackIsolFilter") || filter("hltEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4TrackIsoFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT*
electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoT = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17TrackIsolFilter") || filter("hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17TrackIsoFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to Ele8_Mass50
electronTriggerMatchHLTEle8Mass50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'filter("hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8PMMassFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to SC4_Mass50
electronTriggerMatchHLTSC4Mass50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'filter("hltEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4PMMassFilter")' ),
							   maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to SC17_Mass50
electronTriggerMatchHLTSC17Mass50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17PMMassFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass50_v*
photonTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8Mass50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass50_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v*
photonTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4Mass50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_Mass50_v*
photonTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17Mass50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_Mass50_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT*
photonTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVT = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsolFilter") || filter("hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsoFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT
photonTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVT = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
							   matchedCuts = cms.string( 'filter("hltEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4TrackIsolFilter") || filter("hltEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4TrackIsoFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT*
photonTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoT = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17TrackIsolFilter") || filter("hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17TrackIsoFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to Ele8_Mass50
photonTriggerMatchHLTEle8Mass50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8PMMassFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to SC4_Mass50
photonTriggerMatchHLTSC4Mass50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4PMMassFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matche to SC17_Mass50
photonTriggerMatchHLTSC17Mass50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'filter("hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17PMMassFilter")' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )
# match to HLT_Photon20_CaloIdVL_IsoL_v*
photonTriggerMatchHLTPhoton20CaloIdVLIsoL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon20_CaloIdVL_IsoL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Photon26_Photon18_v*
photonTriggerMatchHLTPhoton26Photon18 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon26_Photon18_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )


# match to HLT_Photon30_CaloIdVL_v*
photonTriggerMatchHLTPhoton30CaloIdVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon30_CaloIdVL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Photon30_CaloIdVL_IsoL_v*
photonTriggerMatchHLTPhoton30CaloIdVLIsoL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon30_CaloIdVL_IsoL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Photon36_Photon22_v*
photonTriggerMatchHLTPhoton36Photon22 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon36_Photon22_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )
# match to HLT_Photon36_CaloId10_Iso50_Photon22_CaloId10_Iso50_v*
photonTriggerMatchHLTPhoton36CaloId10Iso50Photon22CaloId10Iso50 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon36_CaloId10_Iso50_Photon22_CaloId10_Iso50_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Photon50_CaloIdVL_v*
photonTriggerMatchHLTPhoton50CaloIdVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon50_CaloIdVL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Photon50_CaloIdVL_IsoL_v*
photonTriggerMatchHLTPhoton50CaloIdVLIsoL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon50_CaloIdVL_IsoL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Photon75_CaloIdVL_v*
photonTriggerMatchHLTPhoton75CaloIdVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon75_CaloIdVL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Photon75_CaloIdVL_IsoL_v*
photonTriggerMatchHLTPhoton75CaloIdVLIsoL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon75_CaloIdVL_IsoL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Photon90_CaloIdVL_v*
photonTriggerMatchHLTPhoton90CaloIdVL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon90_CaloIdVL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Photon90_CaloIdVL_IsoL_v*
photonTriggerMatchHLTPhoton90CaloIdVLIsoL = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon90_CaloIdVL_IsoL_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Photon135_v*
photonTriggerMatchHLTPhoton135 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon135_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# match to HLT_Photon150_v*
photonTriggerMatchHLTPhoton150 = cms.EDProducer( "PATTriggerMatcherDRLessByR",
							   src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_Photon150_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_PFJet40_v*
jetTriggerMatchHLTPFJet40   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_PFJet40_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_PFJet80_v*
jetTriggerMatchHLTPFJet80   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_PFJet80_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_PFJet140_v*
jetTriggerMatchHLTPFJet140   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_PFJet140_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_PFJet200_v*
jetTriggerMatchHLTPFJet200   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_PFJet200_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_PFJet260_v*
jetTriggerMatchHLTPFJet260   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_PFJet260_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_PFJet320_v*
jetTriggerMatchHLTPFJet320   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_PFJet320_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_PFJet400_v*
jetTriggerMatchHLTPFJet400   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           matchedCuts = cms.string( 'path( "HLT_PFJet400_v*", 1, 0 )' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

vgTriggerMatcherElectron = cms.Sequence(
	electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL +
	electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVL +
	electronTriggerMatchHLTEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL +
	electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8Mass50 +
	electronTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4Mass50 +
	electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17Mass50 +
	electronTriggerMatchHLTEle27WP80PFMETMT50 +
	electronTriggerMatchHLTEle27WP80 +
	electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVT +
	electronTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVT +
	electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoT +
	electronTriggerMatchHLTEle8Mass50 +
	electronTriggerMatchHLTSC4Mass50 +
	electronTriggerMatchHLTSC17Mass50
        )

vgTriggerMatcherPhoton = cms.Sequence(
	photonTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8Mass50 +
        photonTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4Mass50 +
        photonTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17Mass50 +
	photonTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVT +
        photonTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVT +
	photonTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoT +
        photonTriggerMatchHLTEle8Mass50 +
        photonTriggerMatchHLTSC4Mass50 +
        photonTriggerMatchHLTSC17Mass50 +
	photonTriggerMatchHLTPhoton20CaloIdVLIsoL +
	photonTriggerMatchHLTPhoton26Photon18 +
	photonTriggerMatchHLTPhoton30CaloIdVL +
	photonTriggerMatchHLTPhoton30CaloIdVLIsoL +
	photonTriggerMatchHLTPhoton36Photon22 +
	photonTriggerMatchHLTPhoton36CaloId10Iso50Photon22CaloId10Iso50 +
	photonTriggerMatchHLTPhoton50CaloIdVL +
	photonTriggerMatchHLTPhoton50CaloIdVLIsoL +
	photonTriggerMatchHLTPhoton75CaloIdVL +
	photonTriggerMatchHLTPhoton75CaloIdVLIsoL +
	photonTriggerMatchHLTPhoton90CaloIdVL +
	photonTriggerMatchHLTPhoton90CaloIdVLIsoL +
	photonTriggerMatchHLTPhoton135 +
	photonTriggerMatchHLTPhoton150
	)

vgTriggerMatcherMuon = cms.Sequence(
    muonTriggerMatchHLTIsoMu24 +
    muonTriggerMatchHLTIsoMu30 +
    muonTriggerMatchHLTIsoMu24eta2p1 +
    muonTriggerMatchHLTIsoMu30eta2p1 +
    muonTriggerMatchHLTIsoMu34eta2p1 +
    muonTriggerMatchHLTMu17Mu8 +
    muonTriggerMatchHLTMu17TkMu8 +
    muonTriggerMatchHLTMu17forMu17Mu8 +
    muonTriggerMatchHLTMu17forMu17TkMu8 +
    muonTriggerMatchHLTMu8forMu17Mu8 +
    muonTriggerMatchHLTTkMu8forMu17TkMu8
    )

vgTriggerMatcherJet = cms.Sequence(
	jetTriggerMatchHLTPFJet40 + 
	jetTriggerMatchHLTPFJet80 + 
	jetTriggerMatchHLTPFJet140 + 
	jetTriggerMatchHLTPFJet200 + 
	jetTriggerMatchHLTPFJet260 + 
	jetTriggerMatchHLTPFJet320 + 
	jetTriggerMatchHLTPFJet400 
	)

vgTriggerMatcher = cms.Sequence(
    vgTriggerMatcherElectron + vgTriggerMatcherMuon + vgTriggerMatcherPhoton + vgTriggerMatcherJet
    )
