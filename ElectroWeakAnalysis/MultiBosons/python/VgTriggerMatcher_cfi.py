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
                                      maxDeltaR = cms.double( 0.3 ),
                                      resolveAmbiguities    = cms.bool( True ),
                                      resolveByMatchQuality = cms.bool( True )
                                      )

# matches to HLT_Mu11
muonTriggerMatchHLTMu11 = cms.EDProducer("PATTriggerMatcherDRDPtLessByR",
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
                                                  maxDeltaR = cms.double( 0.3 ),
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
                                                  maxDeltaR = cms.double( 0.3 ),
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
                                                  maxDeltaR = cms.double( 0.3 ),
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
                                                  maxDeltaR = cms.double( 0.3 ),
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
                                                           maxDeltaR = cms.double( 0.3 ),
                                                           resolveAmbiguities    = cms.bool( True ),
                                                           resolveByMatchQuality = cms.bool( True )
                                                           )

# matches to HLT_Ele17_SW_CaloEleId_L1R 
electronTriggerMatchHLTEle17SWCaloEleIdL1R  = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
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
electronTriggerMatchHLTEle17SWTightEleIdL1R   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
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

# matches to HLT_Photon10_Cleaned_L1R
photonTriggerMatchHLTPhoton10CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
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
photonTriggerMatchHLTPhoton15CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
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
photonTriggerMatchHLTPhoton20CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
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
photonTriggerMatchHLTPhoton30CleanedL1R   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
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

# matches to HLT_Jet15U
jetTriggerMatchHLTJet15U   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
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
jetTriggerMatchHLTJet30U   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
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
jetTriggerMatchHLTJet50U   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
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
jetTriggerMatchHLTJet70U   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
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

# matches to HLT_Jet100U
jetTriggerMatchHLTJet100U   = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
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

vgTriggerMatcherElectron = cms.Sequence(
        electronTriggerMatchHLTEle15LWL1R +
        electronTriggerMatchHLTEle15SWL1R +
        electronTriggerMatchHLTEle20SWL1R +
        electronTriggerMatchHLTEle15SWEleIdL1R +
        electronTriggerMatchHLTEle15SWCaloEleIdL1R +
        electronTriggerMatchHLTEle17SWCaloEleIdL1R +
        electronTriggerMatchHLTEle17SWTightEleIdL1R
        )

vgTriggerMatcherPhoton = cms.Sequence(
	photonTriggerMatchHLTPhoton10CleanedL1R +
	photonTriggerMatchHLTPhoton15CleanedL1R +
	photonTriggerMatchHLTPhoton20CleanedL1R +
	photonTriggerMatchHLTPhoton30CleanedL1R
	)

vgTriggerMatcherMuon = cms.Sequence(
    muonTriggerMatchHLTMu9 +
    muonTriggerMatchHLTMu11     
    )

vgTriggerMatcherJet = cms.Sequence(
	jetTriggerMatchHLTJet15U +
	jetTriggerMatchHLTJet30U +
	jetTriggerMatchHLTJet50U +
	jetTriggerMatchHLTJet70U +
	jetTriggerMatchHLTJet100U
	)

vgTriggerMatcher = cms.Sequence(
    vgTriggerMatcherElectron + vgTriggerMatcherMuon + vgTriggerMatcherPhoton + vgTriggerMatcherJet
    )
