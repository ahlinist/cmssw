import FWCore.ParameterSet.Config as cms

# matches to HLT_Mu9
#muonTriggerMatchHLTMu9 = cms.EDProducer("PATTriggerMatcherDRDPtLessByR",
muonTriggerMatchHLTMu9 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      andOr          = cms.bool( False ),
                                      filterIdsEnum  = cms.vstring( '*' ),
                                      filterIds      = cms.vint32( 0 ),
                                      filterLabels   = cms.vstring( '*' ),
                                      pathNames      = cms.vstring( 'HLT_Mu9' ),
                                      collectionTags = cms.vstring( '*' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu11
#muonTriggerMatchHLTMu11 = cms.EDProducer("PATTriggerMatcherDRDPtLessByR",
muonTriggerMatchHLTMu11 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      andOr          = cms.bool( False ),
                                      filterIdsEnum  = cms.vstring( '*' ),
                                      filterIds      = cms.vint32( 0 ),
                                      filterLabels   = cms.vstring( '*' ),
                                      pathNames      = cms.vstring( 'HLT_Mu11' ),
                                      collectionTags = cms.vstring( '*' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu13
#muonTriggerMatchHLTMu13 = cms.EDProducer("PATTriggerMatcherDRDPtLessByR",
muonTriggerMatchHLTMu13 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      andOr          = cms.bool( False ),
                                      filterIdsEnum  = cms.vstring( '*' ),
                                      filterIds      = cms.vint32( 0 ),
                                      filterLabels   = cms.vstring( '*' ),
                                      pathNames      = cms.vstring( 'HLT_Mu13' ),
                                      collectionTags = cms.vstring( '*' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu13_v1
#muonTriggerMatchHLTMu13v1 = cms.EDProducer("PATTriggerMatcherDRDPtLessByR",
muonTriggerMatchHLTMu13v1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      andOr          = cms.bool( False ),
                                      filterIdsEnum  = cms.vstring( '*' ),
                                      filterIds      = cms.vint32( 0 ),
                                      filterLabels   = cms.vstring( '*' ),
                                      pathNames      = cms.vstring( 'HLT_Mu13_v1' ),
                                      collectionTags = cms.vstring( '*' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu15
#muonTriggerMatchHLTMu15 = cms.EDProducer("PATTriggerMatcherDRDPtLessByR",
muonTriggerMatchHLTMu15 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      andOr          = cms.bool( False ),
                                      filterIdsEnum  = cms.vstring( '*' ),
                                      filterIds      = cms.vint32( 0 ),
                                      filterLabels   = cms.vstring( '*' ),
                                      pathNames      = cms.vstring( 'HLT_Mu15' ),
                                      collectionTags = cms.vstring( '*' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu15_v1
#muonTriggerMatchHLTMu15v1 = cms.EDProducer("PATTriggerMatcherDRDPtLessByR",
muonTriggerMatchHLTMu15v1 = cms.EDProducer("PATTriggerMatcherDRLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      andOr          = cms.bool( False ),
                                      filterIdsEnum  = cms.vstring( '*' ),
                                      filterIds      = cms.vint32( 0 ),
                                      filterLabels   = cms.vstring( '*' ),
                                      pathNames      = cms.vstring( 'HLT_Mu15_v1' ),
                                      collectionTags = cms.vstring( '*' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Ele15_LW_L1R
#electronTriggerMatchHLTEle15LWL1R = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle15LWL1R = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
                                                  andOr          = cms.bool( False ),
                                                  filterIdsEnum  = cms.vstring( '*' ),
                                                  filterIds      = cms.vint32( 0 ),
                                                  filterLabels   = cms.vstring( '*' ),
                                                  pathNames      = cms.vstring( 'HLT_Ele15_LW_L1R' ),
                                                  collectionTags = cms.vstring( '*' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele15_SW_L1R
#electronTriggerMatchHLTEle15SWL1R = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle15SWL1R = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
                                                  andOr          = cms.bool( False ),
                                                  filterIdsEnum  = cms.vstring( '*' ),
                                                  filterIds      = cms.vint32( 0 ),
                                                  filterLabels   = cms.vstring( '*' ),
                                                  pathNames      = cms.vstring( 'HLT_Ele15_SW_L1R' ),
                                                  collectionTags = cms.vstring( '*' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele20_SW_L1R
#electronTriggerMatchHLTEle20SWL1R = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle20SWL1R = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
                                                  andOr          = cms.bool( False ),
                                                  filterIdsEnum  = cms.vstring( '*' ),
                                                  filterIds      = cms.vint32( 0 ),
                                                  filterLabels   = cms.vstring( '*' ),
                                                  pathNames      = cms.vstring( 'HLT_Ele20_SW_L1R' ),
                                                  collectionTags = cms.vstring( '*' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele15_SW_EleId_L1R
#electronTriggerMatchHLTEle15SWEleIdL1R = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle15SWEleIdL1R = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
                                                  andOr          = cms.bool( False ),
                                                  filterIdsEnum  = cms.vstring( '*' ),
                                                  filterIds      = cms.vint32( 0 ),
                                                  filterLabels   = cms.vstring( '*' ),
                                                  pathNames      = cms.vstring( 'HLT_Ele15_SW_EleId_L1R' ),
                                                  collectionTags = cms.vstring( '*' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.3 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele15_SW_CaloEleId_L1R
#electronTriggerMatchHLTEle15SWCaloEleIdL1R = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle15SWCaloEleIdL1R = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Ele15_SW_CaloEleId_L1R' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_CaloEleId_L1R 
#electronTriggerMatchHLTEle17SWCaloEleIdL1R  = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle17SWCaloEleIdL1R  = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Ele17_SW_CaloEleId_L1R' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_TightEleId_L1R 
#electronTriggerMatchHLTEle17SWTightEleIdL1R   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle17SWTightEleIdL1R   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Ele17_SW_TightEleId_L1R' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1
#electronTriggerMatchHLTEle17SWTightCaloEleIdEle8HEL1Rv1   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle17SWTightCaloEleIdEle8HEL1Rv1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2
#electronTriggerMatchHLTEle17SWTightCaloEleIdEle8HEL1Rv2   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle17SWTightCaloEleIdEle8HEL1Rv2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_TighterEleIdIsol_L1R 
#electronTriggerMatchHLTEle17SWTighterEleIdIsolL1R    = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle17SWTighterEleIdIsolL1R    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Ele17_SW_TighterEleIdIsol_L1R' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_TighterEleIdIsol_L1R_v2
#electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv2    = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv2    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Ele17_SW_TighterEleIdIsol_L1R_v2' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_TighterEleIdIsol_L1R_v3
#electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv3    = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv3    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Ele17_SW_TighterEleIdIsol_L1R_v3' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele22_SW_TighterEleId_L1R_v2
#electronTriggerMatchHLTEle22SWTighterEleIdL1Rv2    = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
electronTriggerMatchHLTEle22SWTighterEleIdL1Rv2    = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ), 
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Ele22_SW_TighterEleId_L1R_v2' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Photon10_Cleaned_L1R
#photonTriggerMatchHLTPhoton10CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
photonTriggerMatchHLTPhoton10CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Photon10_Cleaned_L1R' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Photon15_Cleaned_L1R
#photonTriggerMatchHLTPhoton15CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
photonTriggerMatchHLTPhoton15CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Photon15_Cleaned_L1R' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Photon20_Cleaned_L1R
#photonTriggerMatchHLTPhoton20CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
photonTriggerMatchHLTPhoton20CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Photon20_Cleaned_L1R' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Photon30_Cleaned_L1R
#photonTriggerMatchHLTPhoton30CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
photonTriggerMatchHLTPhoton30CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Photon30_Cleaned_L1R' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Photon50_Cleaned_L1R_v1
#photonTriggerMatchHLTPhoton50CleanedL1Rv1   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
photonTriggerMatchHLTPhoton50CleanedL1Rv1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Photon50_Cleaned_L1R_v1' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Photon70_Cleaned_L1R_v1
#photonTriggerMatchHLTPhoton70CleanedL1Rv1   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
photonTriggerMatchHLTPhoton70CleanedL1Rv1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Photon70_Cleaned_L1R_v1' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_DoublePhoton17_L1R
#photonTriggerMatchHLTDoublePhoton17L1R = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
photonTriggerMatchHLTDoublePhoton17L1R = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                         src     = cms.InputTag( "cleanPatPhotons" ),
                                                         matched = cms.InputTag( "patTrigger" ),
                                                         andOr          = cms.bool( False ),
                                                         filterIdsEnum  = cms.vstring( '*' ),
                                                         filterIds      = cms.vint32( 0 ),
                                                         filterLabels   = cms.vstring( '*' ),
                                                         pathNames      = cms.vstring( 'HLT_DoublePhoton17_L1R' ),
                                                         collectionTags = cms.vstring( '*' ),
                                                         maxDPtRel = cms.double( 0.5 ),
                                                         maxDeltaR = cms.double( 0.3 ),
                                                         resolveAmbiguities    = cms.bool( True ),
                                                         resolveByMatchQuality = cms.bool( True )
                                                         )

# matches to HLT_Photon10_L1R
#photonTriggerMatchHLTPhoton10L1R   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
photonTriggerMatchHLTPhoton10L1R   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatPhotons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Photon10_L1R' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet15U
#jetTriggerMatchHLTJet15U   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet15U   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet15U' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet30U
#jetTriggerMatchHLTJet30U   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet30U   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet30U' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet50U
#jetTriggerMatchHLTJet50U   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet50U   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet50U' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet70U
#jetTriggerMatchHLTJet70U   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet70U   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet70U' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet70U_v2
#jetTriggerMatchHLTJet70Uv2   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet70Uv2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet70U_v2' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet100U
#jetTriggerMatchHLTJet100U   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet100U   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet100U' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet100U_v2
#jetTriggerMatchHLTJet100Uv2   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet100Uv2   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet100U_v2' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet140U_v1
#jetTriggerMatchHLTJet140Uv1   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet140Uv1   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet140U_v1' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet15U_v3
#jetTriggerMatchHLTJet15Uv3   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet15Uv3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet15U_v3' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet30U_v3
#jetTriggerMatchHLTJet30Uv3   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet30Uv3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet30U_v3' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet50U_v3
#jetTriggerMatchHLTJet50Uv3   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet50Uv3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet50U_v3' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet70U_v3
#jetTriggerMatchHLTJet70Uv3   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet70Uv3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet70U_v3' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet100U_v3
#jetTriggerMatchHLTJet100Uv3   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet100Uv3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet100U_v3' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Jet140U_v3
#jetTriggerMatchHLTJet140Uv3   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
jetTriggerMatchHLTJet140Uv3   = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                           src     = cms.InputTag( "cleanPatJets" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Jet140U_v3' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

vgTriggerMatcherElectron = cms.Sequence(
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
	electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv3
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
    muonTriggerMatchHLTMu15v1
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
	jetTriggerMatchHLTJet140Uv3
	)

vgTriggerMatcher = cms.Sequence(
    vgTriggerMatcherElectron + vgTriggerMatcherMuon + vgTriggerMatcherPhoton + vgTriggerMatcherJet
    )
