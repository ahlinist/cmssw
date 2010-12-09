import FWCore.ParameterSet.Config as cms

##### WMuNu group standard selector ####

#from ElectroWeakAnalysis.WMuNu.WMuNuSelection_cff import *


### HLT filter ###
import HLTrigger.HLTfilters.hltHighLevel_cfi
hltFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","REDIGI")
hltFilter.HLTPaths = ['HLT_Mu9']#,'HLT_Ele15_LW_L1R']

#import HLTrigger.HLTfilters.hltHighLevelDev_cfi
#hltFilterDev = HLTrigger.HLTfilters.hltHighLevelDev_cfi.hltHighLevelDev.clone()
#hltFilterDev.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
#hltFilterDev.andOr = True
#hltFilterDev.HLTPaths = ('HLT_Mu9','HLT_Mu9')
#hltFilterDev.HLTPathsPrescales = cms.vuint32(1,1)



hltFilterEle = cms.EDFilter("HLTSummaryFilter",
                            summary = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
                            member = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter","","HLT"),
                            cut = cms.string("pt> 20"),
                            minN = cms.int32(1),
                                 )

#####Primary vertex filter
PVFilter = cms.EDFilter(
    'SingleTopVertexFilter',
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string('!isFake & position().Rho() < 2.0 & abs(z) < 24 & ndof > 4.'),
    filter = cms.bool(True)
    )

PVFilterProducer = cms.EDFilter(
    'VertexSelector',
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string('!isFake & position().Rho() < 2.0 & abs(z) < 24 & ndof > 4.'),
    filter = cms.bool(False)
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

countMuonsPF = countMuons.clone(src = cms.InputTag("topMuons")) 

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

diElectrons = cms.EDProducer("CandViewShallowCloneCombiner",
                             decay = cms.string("topElectrons looseElectrons"),
                             checkCharge = cms.bool(True),
                             cut = cms.string("mass> 76 && mass < 106")
                             ) 

vetoDiElectrons = cms.EDFilter("CandViewCountFilter",
                               src = cms.InputTag("diElectrons"),
                               minNumber = cms.uint32(0),
                               maxNumber = cms.uint32(0),
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

countJetsPF= countJets.clone(
    src = cms.InputTag('topJetsPF')
    )


countBTagsPF = countJets.clone(
    src = cms.InputTag('bJetsPF')
    )

countForwardJetsPF = countJets.clone(
    src = cms.InputTag('forwardJetsPF')
    )

countJetsNonTTBar = countJets.clone(
    src = cms.InputTag('topJets')
    )


countJetsNonTTBarAntiIso = countJets.clone(
    src = cms.InputTag('topJetsAntiIso')
    )

countJetsNonTTBarPF = countJets.clone(
    src = cms.InputTag('topJets')
    )


countJetsNonTTBarAntiIsoPF = countJets.clone(
    src = cms.InputTag('topJetsAntiIso')
    )

countJetsTTBarAntiIso = countJets.clone(
    src = cms.InputTag('topJetsAntiIso')
    )

vetoBJets = countBTags.clone()

countJetsAntiIsoPF= countJets.clone(
    src = cms.InputTag('topJetsAntiIsoPF')
    )

countBTagsAntiIsoPF = countJets.clone(
    src = cms.InputTag('bJetsAntiIsoPF')
    )

countForwardJetsAntiIsoPF = countJets.clone(
    src = cms.InputTag('forwardJetsAntiIsoPF')
    )

countJetsNonTTBarAntiIsoPF = countJets.clone(
    src = cms.InputTag('topJetsAntiIsoPF')
    )





#electronIDIso = cms.EDFilter('SingleTopElectronFilter',
#                          src = cms.InputTag("topElectrons"),
#                          isData = cms.untracked.bool(False),   
#                          )

#electronIDAntiIso = cms.EDFilter('SingleTopElectronFilter',
#                          src = cms.InputTag("topElectronsAntiIso"),
#                          isData = cms.untracked.bool(False),   
#                          )

from CommonTools.RecoAlgos.HBHENoiseFilter_cfi import * 


MTWFilterMuons = cms.EDFilter('SingleTopMTWFilter',
src1 = cms.InputTag("topMuons"),
src2 = cms.InputTag("preselectedMETs"), 
                              )

MTWFilterMuonsPF = MTWFilterMuons.clone(
    src1 = cms.InputTag("topMuons"),
    src2 = cms.InputTag("patMETsPF"),
    cut = cms.untracked.double(40),
    )



MTWFilterElectrons = MTWFilterMuons.clone(src1 = cms.InputTag("topElectrons"),)
MTWFilterElectronsPF = MTWFilterElectrons.clone(src2 = cms.InputTag("patMETsPF"),)
