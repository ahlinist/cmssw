import FWCore.ParameterSet.Config as cms

from TopQuarkAnalysis.SingleTop.SelectionCuts_top_group_control_samples_v2_cff import *

from TopQuarkAnalysis.SingleTop.SingleTopIDTools_cff import *

from PhysicsTools.PatAlgos.patSequences_cff import *



patElectrons.addElectronID = cms.bool(True)
patElectrons.electronIDSources = electronIDSources

patElectronIDs = cms.Sequence(simpleEleIdSequence)

makeNewPatElectrons = cms.Sequence(patElectronIDs * patElectronIsolation * patElectrons)

patElectrons.usePV = cms.bool(False)
patMuons.usePV = cms.bool(False)



basePath = cms.Sequence(
    preselectedJets *
    preselectedMETs *
    preselectedMuons *
    preselectedElectrons 
    )


##Muons Sequences
baseMuonSequence = cms.Sequence(
    hltFilterDev *
    PVFilter *
    countLeptons *
    topMuons *
    countMuons *
    topElectrons 
    )

baseMuonAntiIsoSequence = cms.Sequence(
    hltFilterDev *
    PVFilter *
    vetoLeptonsIso *
    topMuonsAntiIso *
    countMuonsAntiIso *
    topElectronsAntiIso *
    vetoElectronsAntiIso 
    )
##


##Electron sequences
baseElectronSequence = cms.Sequence(
    hltFilterDev *
    PVFilter *
    countLeptons *
#    vetoLooseMuons *
#    vetoZInvariantMass *
    topElectrons *
    countElectrons *
    electronIDIso 
    

    )


baseElectronAntiIsoSequence = cms.Sequence(
    hltFilterDev *
    PVFilter *
    vetoLeptonsIso *
    topElectronsAntiIso *
    countElectronsAntiIso *
    electronIDAntiIso *
    topMuonsAntiIso *
    vetoMuonsAntiIso 
    )


###All leptons, same path for electrons and muons

baseLeptonSequence = cms.Sequence(
    countLeptons *
    topMuons *
    topElectrons * 
    countLeptonsTight
    )

baseLeptonAntiIsoSequence = cms.Sequence(
    topElectrons *
    vetoLeptonsIso *
    countLeptonsAntiIso 
    )


baseJetMETSequence = cms.Sequence(
#Production
    topJets *
    bJets *
    antiBJets *
    forwardJets 
   )

baseJetMETAntiIsoSequence = cms.Sequence(
#Production
    topJetsAntiIso *
    bJetsAntiIso *
    antiBJetsAntiIso *
    forwardJetsAntiIso 
   )

###Muon paths:

IsoMuons = cms.Sequence(
    baseMuonSequence +
    baseJetMETSequence
    )

AntiIsoMuons = cms.Sequence(
    baseMuonAntiIsoSequence +
    baseJetMETAntiIsoSequence
    )

###Electron paths
IsoElectrons = cms.Sequence(
    baseElectronSequence +
    baseJetMETSequence
    )

AntiIsoElectrons = cms.Sequence(
    baseElectronAntiIsoSequence +
    baseJetMETAntiIsoSequence
    )



PathIso = cms.Sequence(
    baseLeptonSequence *
    baseJetMETSequence *
    countJetsNonTTBar
    )

#AntiIso Paths
PathAntiIso = cms.Sequence(
    baseLeptonAntiIsoSequence *
    baseJetMETAntiIsoSequence *
    countJetsNonTTBarAntiIso
    )



###Tops Sequences
allTops= cms.Sequence(
    recoTops *
    boostedTops *
    boostedForwardJets 
    )

allAntiIsoTops= cms.Sequence(
    recoTopsAntiIso *
    boostedTopsAntiIsoTops *
    boostedForwardJetsAntiIsoTops
    )

allPseudoBJetsTops= cms.Sequence(
    recoTopsWSamplePseudoBTags *
    boostedTopsWSamplePseudoBTagsTops *
    boostedForwardJetsWSamplePseudoBTagsTops 
    )

allPseudoBJetsAntiIsoTops= cms.Sequence(
    recoTopsWSamplePseudoBTagsAntiIso *
    boostedTopsWSamplePseudoBTagsAntiIsoTops *
    boostedForwardJetsWSamplePseudoBTagsAntiIsoTops 
    )



TSampleMuon = cms.Sequence(
    IsoMuons *
    countJetsNonTTBar *
    countBTags * 
    countForwardJets * 
    allTops *
    singleTopObservablesTSample #*
#    SingleTopWtransverseMassFilter
    )


WSampleMuon = cms.Sequence(
    IsoMuons *
    countJetsNonTTBar *
    pseudoBJets * 
    countWSamplePseudoBTags * 
    countForwardJets * 
    allPseudoBJetsTops *
    singleTopObservablesWSample #*
#    SingleTopWtransverseMassFilterWSample

    )

TTBarSampleMuon = cms.Sequence(
    IsoMuons *
    countBTags * 
    countForwardJets * 
    countJetsTTBar *
    allTops *
    singleTopObservablesTTBarSample #*
#    SingleTopWtransverseMassFilterTTBarSample
    )

QCDSampleMuon = cms.Sequence(
    AntiIsoMuons *
    countJetsNonTTBarAntiIso *
    bJetsAntiIso *
    forwardJetsAntiIso *
    countBTagsAntiIso *
    countForwardJetsAntiIso *
    allAntiIsoTops *
    singleTopObservablesAntiIso
    )


QCDWSampleMuon = cms.Sequence(
    AntiIsoMuons *
    countJetsNonTTBarAntiIso *
    pseudoBJetsAntiIso * 
    countWSamplePseudoBTagsAntiIso * 
    countForwardJetsAntiIso * 
    allPseudoBJetsAntiIsoTops *
    singleTopObservablesWSampleAntiIso 
    )




TSampleElectron = cms.Sequence(
    IsoElectrons *
    countJetsNonTTBar *
    countBTags * 
    countForwardJets * 
    allTops *
    singleTopObservablesTSample #*
#    SingleTopWtransverseMassFilter
    )


WSampleElectron = cms.Sequence(
    IsoElectrons *
    countJetsNonTTBar *
    pseudoBJets * 
    countWSamplePseudoBTags * 
    countForwardJets * 
    allPseudoBJetsTops *
    singleTopObservablesWSample #*
#    SingleTopWtransverseMassFilterWSample

    )

TTBarSampleElectron = cms.Sequence(
    IsoElectrons *
    countBTags * 
    countForwardJets * 
    countJetsTTBar *
    allTops *
    singleTopObservablesTTBarSample #*
#    SingleTopWtransverseMassFilterTTBarSample
    )

QCDSampleElectron = cms.Sequence(
    AntiIsoElectrons *
    countJetsNonTTBarAntiIso *
    bJetsAntiIso *
    forwardJetsAntiIso *
    countBTagsAntiIso *
    countForwardJetsAntiIso *
    allAntiIsoTops *
    singleTopObservablesAntiIso
    )


QCDWSampleElectron = cms.Sequence(
    AntiIsoElectrons *
    countJetsNonTTBarAntiIso *
    pseudoBJetsAntiIso * 
    countWSamplePseudoBTagsAntiIso * 
    countForwardJetsAntiIso * 
    allPseudoBJetsAntiIsoTops *
    singleTopObservablesWSampleAntiIso 
    )






