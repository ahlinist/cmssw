import FWCore.ParameterSet.Config as cms

from TopQuarkAnalysis.SingleTop.SelectionCuts_top_group_control_samples_v3_cff import *

from PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi import *

from PhysicsTools.PatAlgos.patSequences_cff import *


#cFlavorHistoryProducer.matchedSrc = cms.InputTag("antikt5GenJets")
#bFlavorHistoryProducer.matchedSrc = cms.InputTag("antikt5GenJets")

#patElectrons.addElectronID = cms.bool(True)
#patElectrons.electronIDSources = electronIDSources

#patElectronIDs = cms.Sequence(simpleEleIdSequence)

#makeNewPatElectrons = cms.Sequence(patElectronIDs * patElectronIsolation * patElectrons)

patElectrons.usePV = cms.bool(False)
patMuons.usePV = cms.bool(False)


basePath = cms.Sequence(
   preselectedMETs *
   preselectedMuons *
   preselectedElectrons *
   looseElectrons
   )

flavorHistorySequence = cms.Sequence(
    cFlavorHistoryProducer *
    bFlavorHistoryProducer
    )

##Muons Sequences
baseMuonSequencePF = cms.Sequence(
    hltFilter *
    PVFilter *
    countLeptons *
    topElectrons *
    topMuons *
    preselectedJets *
    topJetsPF *
    countMuons 
    )


baseMuonSequence = cms.Sequence(
    hltFilter *
    PVFilter *
    HBHENoiseFilter *
    countLeptons *
    topMuons *
    topElectrons *
    preselectedJets *
    topJets *
    #topJetsPF *
    countMuons 
    )

baseMuonSequencePF = cms.Sequence(
    hltFilter *
    PVFilter *
    HBHENoiseFilter *
    countLeptons *
    topElectrons *
    topMuons *
    preselectedJets *
    #    topJets *
    topJetsPF *
    countMuonsPF 
    )

baseMuonAntiIsoSequence = cms.Sequence(
    hltFilter *
    PVFilter *
    vetoLeptonsIso *
    topMuonsAntiIso *
    countMuonsAntiIso *
    preselectedJets *
    topElectronsAntiIso *
    vetoElectronsAntiIso 
    )
##


hltFilterEleSequence = cms.Sequence( hltFilterEle )


##Electron sequences
baseElectronSequence = cms.Sequence(
    PVFilter *
    HBHENoiseFilter *
    #    countLeptons *
    vetoLooseMuons *
    topElectrons *
    diElectrons *
    vetoDiElectrons *
    countElectrons *
#    electronIDIso *
    topMuons *
    preselectedJets 
    )

baseElectronSequencePF = cms.Sequence(
     PVFilter *
    HBHENoiseFilter *
    #    countLeptons *
    vetoLooseMuons *
    topElectrons *
    diElectrons *
    vetoDiElectrons *
    countElectrons *
#    electronIDIso *
    topMuons *
    preselectedJets 
    )



baseElectronAntiIsoSequence = cms.Sequence(
    PVFilter *
    HBHENoiseFilter *
    vetoLeptonsIso *
    topElectronsAntiIso *
    countElectronsAntiIso *
    #electronIDAntiIso *
    topMuonsAntiIso *
    vetoMuonsAntiIso *
    preselectedJets 
    )

baseElectronAntiIsoSequencePF = cms.Sequence(
    PVFilter *
    HBHENoiseFilter *
    vetoLeptonsIso *
    topElectronsAntiIso *
    countElectronsAntiIso *
    #electronIDAntiIso *
    topMuonsAntiIso *
    vetoMuonsAntiIso *
    preselectedJets 
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
    forwardJets 
   )

baseJetMETSequencePF= cms.Sequence(
#Production
    topJetsPF *
    bJetsPF *
    forwardJetsPF 
   )

baseJetMETAntiIsoSequence = cms.Sequence(
#Production
    topJetsAntiIso *
    bJetsAntiIso *
    antiBJetsAntiIso *
    forwardJetsAntiIso 
   )

baseJetMETAntiIsoSequencePF = cms.Sequence(
#Production
    topJetsAntiIsoPF *
    bJetsAntiIsoPF *
    forwardJetsAntiIsoPF 
   )

baseJetMETAntiIsoSequencePF = cms.Sequence(
#Production
    topJetsAntiIsoPF *
    bJetsAntiIsoPF *
    forwardJetsAntiIsoPF 
   )

###Muon paths:

IsoMuons = cms.Sequence(
    baseMuonSequence +
    baseJetMETSequence
    )

IsoMuonsPF = cms.Sequence(
    baseMuonSequencePF +
    baseJetMETSequencePF
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

IsoElectronsPF = cms.Sequence(
    baseElectronSequence +
    baseJetMETSequencePF
    )

AntiIsoElectrons = cms.Sequence(
    baseElectronAntiIsoSequence +
    baseJetMETAntiIsoSequence
    )


PathMuonsIso = cms.Sequence(
    IsoMuons *
    countJetsNonTTBar
    )

#AntiIso Paths
PathMuonsAntiIso = cms.Sequence(
    AntiIsoMuons *
    countJetsNonTTBarAntiIso
    )



PathElectronsIso = cms.Sequence(
    IsoElectrons *
    countJetsNonTTBar
    )

#AntiIso Paths
PathElectronsAntiIso = cms.Sequence(
    AntiIsoElectrons *
    countJetsNonTTBarAntiIso
    )



###Tops Sequences
allTops= cms.Sequence(
    recoTops *
    recoTChanEvents# *
  #  boostedTops *
  #  boostedForwardJets * 
  #  boostedEvents
    )

allTopsPF = cms.Sequence(
    recoTopsPF *
    recoTChanEventsPF #*
#    boostedTopsPF *
#    boostedForwardJetsPF *
#    boostedEventsPF
    )

allAntiIsoTops= cms.Sequence(
    recoTopsAntiIso *
    recoTChanEventsAntiIso #*
#    boostedTopsAntiIsoTops *
#    boostedForwardJetsAntiIsoTops *
#    boostedEventsAntiIso
    )

allAntiIsoTopsPF= cms.Sequence(
    recoTopsAntiIsoPF *
    recoTChanEventsAntiIsoPF #*
#    boostedEventsAntiIsoPF
    #*
#    boostedTopsAntiIsoTops *
#    boostedForwardJetsAntiIsoTops
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


TSampleMuonPF = cms.Sequence(
    IsoMuonsPF *
    countJetsPF *
    #MTWFilterMuonsPF * 
    countBTagsPF * 
    countForwardJetsPF *
    allTopsPF * 
    singleTopObservablesTSamplePF #*
    )


TSampleMuon = cms.Sequence(
    IsoMuons *
    countJetsNonTTBar *
    MTWFilterMuons * 
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

QCDSampleMuonPF = cms.Sequence(
    AntiIsoMuons *
    baseJetMETAntiIsoSequencePF *    
    countJetsNonTTBarAntiIsoPF *
    bJetsAntiIsoPF *
    forwardJetsAntiIsoPF *
    countBTagsAntiIsoPF *
    countForwardJetsAntiIsoPF * 
    allAntiIsoTopsPF 
#    singleTopObservablesAntiIsoPF
    )

QCDSampleElectronPF = cms.Sequence(
    AntiIsoElectrons *
    baseJetMETAntiIsoSequencePF *    
    countJetsNonTTBarAntiIsoPF *
    bJetsAntiIsoPF *
    forwardJetsAntiIsoPF *
    countBTagsAntiIsoPF *
    countForwardJetsAntiIsoPF *
    allAntiIsoTopsPF 
#    Singletopobservablesantiisopf
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
    topMuons *
    countJetsNonTTBar *
    MTWFilterElectrons * 
    countBTags * 
    countForwardJets * 
    allTops *
    singleTopObservablesTSample #*
#    SingleTopWtransverseMassFilter
    )

TSampleElectronPF = cms.Sequence(
    IsoElectronsPF *
    countJetsPF *
    #MTWFilterElectronsPF * 
    countBTagsPF * 
    countForwardJetsPF * 
    allTopsPF *
    singleTopObservablesTSamplePF #*
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






