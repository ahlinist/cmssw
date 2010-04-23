import FWCore.ParameterSet.Config as cms

######### Part of selection: Particle counting ##########
countElectrons = cms.EDFilter("PATCandViewCountFilter",
                              src = cms.InputTag("topElectrons"),
                              minNumber = cms.uint32(1),
                              maxNumber = cms.uint32(9999),
                              filter = cms.bool(True)
                              )

countMuons = cms.EDFilter("PATCandViewCountFilter",
                          src = cms.InputTag("topMuons"),
                          minNumber = cms.uint32(1),
                          maxNumber = cms.uint32(9999),
                          filter = cms.bool(True)
                          )

countLeptons = cms.EDFilter("CandOrCounter",
                            src1 = cms.InputTag("preselectedMuons"),
                            src2 = cms.InputTag("preselectedElectrons"),
                            
                            minNumber = cms.int32(1),
                            maxNumber = cms.int32(9999),
                            )


countJets = cms.EDFilter("PATCandViewCountFilter",
                         src = cms.InputTag("topJets"),
                         minNumber = cms.uint32(1),
                         maxNumber = cms.uint32(9999),
                         filter = cms.bool(True)
                         )

                              
countBTags = cms.EDFilter("PATCandViewCountFilter",
                          src = cms.InputTag("bJets"),
                          minNumber = cms.uint32(0),
                          maxNumber = cms.uint32(9999),
                          filter = cms.bool(True)
                          )

countAntiBTags = cms.EDFilter("PATCandViewCountFilter",
                             src = cms.InputTag("antiBJets"),
                             minNumber = cms.uint32(0),
                             maxNumber = cms.uint32(9999),
                             filter = cms.bool(True)
                             )

countForwardJets = cms.EDFilter("PATCandViewCountFilter",
                            src = cms.InputTag("forwardJets"),
                            minNumber = cms.uint32(0),
                            maxNumber = cms.uint32(9999),
                            filter = cms.bool(True)                                  
                            )

selectSingleTops = cms.EDFilter("SingleTopSelector",
                                forwardJetsSource = cms.InputTag("forwardJets"),
                                topsSource = cms.InputTag("recoTops")
                                )

