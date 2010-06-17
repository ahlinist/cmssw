import FWCore.ParameterSet.Config as cms

##### WMuNu group standard selector ####

from ElectroWeakAnalysis.WMuNu.WMuNuSelection_cff import *


### HLT filter ###
import HLTrigger.HLTfilters.hltHighLevel_cfi
hltFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
hltFilter.HLTPaths = ['HLT_Mu9','HLT_Ele15_LW_L1R']

#####Primary vertex filter
PVFilter = cms.EDFilter(
    'VertexSelector',
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string('!isFake & position().Rho() < 2.0 & abs(z) < 15 & ndof < 4.')
    )



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

countLeptonsTight = cms.EDFilter("CandOrCounter",
                            src1 = cms.InputTag("topMuons"),
                            src2 = cms.InputTag("topElectrons"),
                            
                            minNumber = cms.int32(1),
                            maxNumber = cms.int32(1),
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


#Part for control samples:

#AntiIso cuts
countLeptonsAntiIso = countLeptons.clone(
    src1 = cms.InputTag("topMuonsAntiIso"),
    src2 = cms.InputTag("topElectronsAntiIso"),
    #src1 = cms.InputTag("preselectedMuonsAntiIso"),
    #src2 = cms.InputTag("preselectedElectronsAntiIso"),
) 


vetoLeptonsIso = countLeptons.clone(
    src1 = cms.InputTag("preselectedMuons"),
    src2 = cms.InputTag("preselectedElectrons"),
) 



vetoLooseMuons = countMuons.clone(
    src = cms.InputTag("preselectedMuons"),
    minNumber = cms.uint32(0),
    maxNumber = cms.uint32(0),
    )

countMuonsAntiIso = countMuons.clone(
    src = cms.InputTag("topMuonsAntiIso"),
    )   

countElectronsAntiIso = countElectrons.clone(
    src = cms.InputTag("topElectronsAntiIso"),
    )


vetoMuonsAntiIso = countMuonsAntiIso.clone(
    minNumber = cms.uint32(0),
    maxNumber = cms.uint32(0),
    )

vetoElectronsAntiIso = countElectronsAntiIso.clone(
    minNumber = cms.uint32(0),
    maxNumber = cms.uint32(0),
    )



countJetsAntiIso = countJets.clone(
    src = cms.InputTag('topJetsAntiIso')
    )

countBTagsAntiIso = countJets.clone(
    src = cms.InputTag('bJetsAntiIso')
    )

countForwardJetsAntiIso = countJets.clone(
    src = cms.InputTag('forwardJetsAntiIso')
    )

countJetsTTBar = countJets.clone(
    src = cms.InputTag('topJets')
    )

countJetsNonTTBar = countJets.clone(
    src = cms.InputTag('topJets')
    )


countJetsNonTTBarAntiIso = countJets.clone(
    src = cms.InputTag('topJetsAntiIso')
    )

countJetsTTBarAntiIso = countJets.clone(
    src = cms.InputTag('topJetsAntiIso')
    )

vetoBJets = countBTags.clone()


from TopQuarkAnalysis.SingleTop.SingleTopIDTools_cff import *




electronIDIso = cms.EDFilter('SingleTopElectronFilter',
                          src = cms.InputTag("topElectrons"),
                          )

electronIDAntiIso = cms.EDFilter('SingleTopElectronFilter',
                          src = cms.InputTag("topElectronsAntiIso"),
                          )
