import FWCore.ParameterSet.Config as cms

# matches to HLT_Mu9
muonTriggerMatchHLTMu9 = cms.EDProducer("PATTriggerMatcherDRDPtLessByR",
                                      src     = cms.InputTag( "cleanPatMuons" ),
                                      matched = cms.InputTag( "patTrigger" ),
                                      andOr          = cms.bool( False ),
                                      filterIdsEnum  = cms.vstring( '*' ),
                                      filterIds      = cms.vint32( 0 ),
                                      filterLabels   = cms.vstring( '*' ),
                                      pathNames      = cms.vstring( 'HLT_Mu9' ),
                                      collectionTags = cms.vstring( '*' ),
                                      maxDPtRel = cms.double( 0.5 ),
                                      maxDeltaR = cms.double( 0.5 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Ele15_LW_L1R
electronTriggerMatchHLTEle15LWL1R = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
                                                  andOr          = cms.bool( False ),
                                                  filterIdsEnum  = cms.vstring( '*' ),
                                                  filterIds      = cms.vint32( 0 ),
                                                  filterLabels   = cms.vstring( '*' ),
                                                  pathNames      = cms.vstring( 'HLT_Ele15_LW_L1R' ),
                                                  collectionTags = cms.vstring( '*' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.5 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele15_SW_L1R
electronTriggerMatchHLTEle15SWL1R = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
                                                  andOr          = cms.bool( False ),
                                                  filterIdsEnum  = cms.vstring( '*' ),
                                                  filterIds      = cms.vint32( 0 ),
                                                  filterLabels   = cms.vstring( '*' ),
                                                  pathNames      = cms.vstring( 'HLT_Ele15_SW_L1R' ),
                                                  collectionTags = cms.vstring( '*' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.5 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele20_SW_L1R
electronTriggerMatchHLTEle20SWL1R = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
                                                  andOr          = cms.bool( False ),
                                                  filterIdsEnum  = cms.vstring( '*' ),
                                                  filterIds      = cms.vint32( 0 ),
                                                  filterLabels   = cms.vstring( '*' ),
                                                  pathNames      = cms.vstring( 'HLT_Ele20_SW_L1R' ),
                                                  collectionTags = cms.vstring( '*' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.5 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele15_SW_EleId_L1R
electronTriggerMatchHLTEle15SWEleIdL1R = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
                                                  src     = cms.InputTag( "cleanPatElectrons" ),
                                                  matched = cms.InputTag( "patTrigger" ),
                                                  andOr          = cms.bool( False ),
                                                  filterIdsEnum  = cms.vstring( '*' ),
                                                  filterIds      = cms.vint32( 0 ),
                                                  filterLabels   = cms.vstring( '*' ),
                                                  pathNames      = cms.vstring( 'HLT_Ele15_SW_EleId_L1R' ),
                                                  collectionTags = cms.vstring( '*' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.5 ),
                                                  resolveAmbiguities    = cms.bool( True ),
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )

# matches to HLT_Ele15_SW_CaloEleId_L1R
electronTriggerMatchHLTEle15SWCaloEleIdL1R = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
                                                           src     = cms.InputTag( "cleanPatElectrons" ),
                                                           matched = cms.InputTag( "patTrigger" ),
                                                           andOr          = cms.bool( False ),
                                                           filterIdsEnum  = cms.vstring( '*' ),
                                                           filterIds      = cms.vint32( 0 ),
                                                           filterLabels   = cms.vstring( '*' ),
                                                           pathNames      = cms.vstring( 'HLT_Ele15_SW_CaloEleId_L1R' ),
                                                           collectionTags = cms.vstring( '*' ),
                                                           maxDPtRel = cms.double( 0.5 ),
                                                           maxDeltaR = cms.double( 0.5 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

vgTriggerMatcherElectron = cms.Sequence(
        electronTriggerMatchHLTEle15LWL1R +
        electronTriggerMatchHLTEle15SWL1R +
        electronTriggerMatchHLTEle20SWL1R +
        electronTriggerMatchHLTEle15SWEleIdL1R +
        electronTriggerMatchHLTEle15SWCaloEleIdL1R
        )

vgTriggerMatcherMuon = cms.Sequence(
    muonTriggerMatchHLTMu9     
    )

vgTriggerMatcher = cms.Sequence(
    vgTriggerMatcherElectron + vgTriggerMatcherMuon    
    )
