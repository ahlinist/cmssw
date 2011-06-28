import FWCore.ParameterSet.Config as cms

isGenHadronFromWtaunuDecays = cms.EDProducer("BoolEventSelFlagProducer",
                                          pluginName = cms.string('isGenHadronFromWtaunuDecays'),
                                          pluginType = cms.string('PATCandViewMinEventSelector'),
                                          src = cms.InputTag('genHadronsFromWtaunuDecays'),
                                          minNumber = cms.uint32(1)
                                          )
isGenNuFromWtaunuDecays = cms.EDProducer("BoolEventSelFlagProducer",
                                         pluginName = cms.string('isGenNuFromWtaunuDecays'),
                                         pluginType = cms.string('PATCandViewMinEventSelector'),
                                         src = cms.InputTag('genNusFromWs'),
                                         minNumber = cms.uint32(1)
                                         )

isGenWtoTauNu = cms.EDProducer("BoolEventSelFlagProducer",
                               pluginName = cms.string('isGenWtoTauNu'),
                               pluginType = cms.string('MultiBoolEventSelFlagSelector'),
                               flags = cms.VInputTag(cms.InputTag('isGenHadronFromWtaunuDecays'),
                                                     cms.InputTag('isGenNuFromWtaunuDecays')
                                                     )
                               )

#-------------------selection within acceptance----------------------------
isGenHadronFromWtaunuDecaysWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
                                                             pluginName = cms.string('isGenHadronFromWtaunuDecaysWithinAcceptance'),
                                                             pluginType = cms.string('PATCandViewMinEventSelector'),
                                                             src = cms.InputTag('genHadronsFromWtaunuDecaysWithinAcceptance'),
                                                             minNumber = cms.uint32(1)
                                                             )
isGenNuFromWtaunuDecaysWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
                                                         pluginName = cms.string('isGenNuFromWtaunuDecaysWithinAcceptnace'),
                                                         pluginType = cms.string('PATCandViewMinEventSelector'),
                                                         src = cms.InputTag('genNusFromWsWithinAcceptance'),
                                                         minNumber = cms.uint32(1)
                                                         )

isGenWtoTauNuWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
                                               pluginName = cms.string('isGenWtoTauNuWithinAcceptance'),
                                               pluginType = cms.string('MultiBoolEventSelFlagSelector'),
                                               flags = cms.VInputTag( cms.InputTag('isGenHadronFromWtaunuDecaysWithinAcceptance'),
                                                                      cms.InputTag('isGenNuFromWtaunuDecaysWithinAcceptance')
                                                                      )
                                               )

selectGenWtaunuDecayModes = cms.Sequence(
    isGenHadronFromWtaunuDecays
    *isGenNuFromWtaunuDecays
    *isGenWtoTauNu
    *isGenHadronFromWtaunuDecaysWithinAcceptance
    *isGenNuFromWtaunuDecaysWithinAcceptance
    *isGenWtoTauNuWithinAcceptance
    )
