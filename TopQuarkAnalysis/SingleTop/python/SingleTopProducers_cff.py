import FWCore.ParameterSet.Config as cms

#muon skim part
preselectedMuons = cms.EDFilter("PATMuonSelector",
  src = cms.InputTag("cleanPatMuons"),
  cut = cms.string('pt > 10 & abs(eta) < 2.1 & isGlobalMuon'),
  filter = cms.bool(False)                                
)

#electron skim part
preselectedElectrons = cms.EDFilter("PATElectronSelector",
  src = cms.InputTag("cleanPatElectrons"),
  cut = cms.string('pt >  20 & abs(eta) < 2.4'),
  filter = cms.bool(False)                                
)

#preselectedJets = cms.EDFilter("PATJetSelector",
#  src = cms.InputTag("cleanPatJets"),
# src = cms.InputTag("myCleanJets"),
#  cut = cms.string('pt >  30 & abs(eta) < 5.0'),
#                            filter = cms.bool(False)                                
#)




#loose second electron skim part
looseElectrons = cms.EDFilter("PATElectronSelector",
  src = cms.InputTag("cleanPatElectrons"),
  cut = cms.string('pt >  20 & abs(eta) < 2.4'),
  filter = cms.bool(False)                                
)

###Cleaning options for jets:
from PhysicsTools.PatAlgos.cleaningLayer1.jetCleaner_cfi import *
preselectedJets = cms.EDProducer("PATJetCleaner",
                                 preselection = cms.string(''),
                                 src = cms.InputTag("cleanPatJets"),
                                 finalCut = cms.string(''),
                                 )

#preselectedJets = cms.EDFilter("PATJetSelector",
#  src = cms.InputTag("cleanPatJets"),
# src = cms.InputTag("myCleanJets"),
#  cut = cms.string('pt >  30 & abs(eta) < 5.0'),
#                            filter = cms.bool(False)                                
#)


#topJets = cms.EDFilter("PATJetSelector",
#  src = cms.InputTag("cleanPatJets"),
#  src = cms.InputTag("myCleanJets"),
#  cut = cms.string('pt >  30 & abs(eta) < 5.0'),
#  filter = cms.bool(False)                                
#)

topJets = cms.EDProducer("SingleTopJetsProducer",
                         src = cms.InputTag("preselectedJets"),
                         eleSrc = cms.InputTag("topElectrons"),
                         muSrc = cms.InputTag("topElectrons"),
                         isJPT = cms.untracked.bool(True),
                         isPF = cms.untracked.bool(False),
                         #  filter = cms.boolFalse)              
                         )

topJetsPF = topJets.clone(
      src = cms.InputTag("selectedPatJetsPFlow"),
      isJPT = cms.untracked.bool(False),
      isPF = cms.untracked.bool(True),
      )


#topJets.checkOverlaps = cms.PSet(
#    myElectrons = cms.PSet(
#    src = cms.InputTag('cleanLayer1Electrons'),
#    src = cms.InputTag('topElectrons'),
#    #   algorithm = cms.string('bySuperClusterSeed'),
#    preselection = cms.string(""),
#    algorithm = cms.string('byDeltaR'),
#    deltaR = cms.double(0.5),
#    checkRecoComponents = cms.bool(False),
#    pairCut =cms.string(""),
#    requireNoOverlaps = cms.bool(False),
#    ),
   
#    myPhotons = cms.PSet(
#    src = cms.InputTag('cleanPatPhotons'),
#    #preselection = cms.string(""),#"pt > 0 & pt/(pt+caloIso) > 0.8"),
#    preselection = cms.string(" pt/(pt+caloIso) > 0.9"),#"pt > 0 & pt/(pt+caloIso) > 0.8"),
#    #   Algorithm = cms.string('bySuperClusterSeed'),
#    algorithm = c#ms.string('byDeltaR'),
#    Deltar = cms.double(0.5), 
#    checkREcocomponents = cms.bool(False),
#    pairCut =cms.string(""),
#   requireNoOverlaps = cms.bool(False),
#    ),
#    )






#Mete skim part
preselectedMETs = cms.EDFilter("PATMETSelector",
  src = cms.InputTag("patMETs"),
  cut = cms.string('pt >   0'),
  filter = cms.bool(False)                                
)

#Part of BJets tagging and Forward Jets production
allTopJets = cms.EDProducer("SingleTopJetsProducer",
                                 jetsSource = cms.InputTag("preselectedJets"),
                                 )

topMuons = cms.EDFilter("PATMuonSelector",
                        src = cms.InputTag("preselectedMuons"),
                        filter = cms.bool(False),
                        cut = cms.string("pt > 0")
                        )

#topMuons = cms.EDProducer("SingleTopMuonProducer",
#                          src = cms.InputTag("preselectedMuons"),
#                          jetSrc = cms.InputTag("topJets")
#                          )

topMuonsPF =  topMuons.clone(jetSrc = cms.InputTag("topJetsPF")) 

topElectrons = cms.EDFilter("PATElectronSelector",
                            src = cms.InputTag("preselectedElectrons"),
                            filter = cms.bool(False),
                            cut = cms.string("pt > 0")
                            )
#topMuons = cms.EDProducer("PATMuonCleaner",
#                          src= cms.InputTag("preselectedMuons"),
#                          preselection = cms.string(''),
#                          checkOverlaps = cms.PSet(
#    myJets = cms.PSet(
#    src = cms.InputTag("preselectedJets"),
#    preselection = cms.string('pt > 20'),
#    algorithm = cms.string('byDeltaR'),
#    deltaR = cms.double(0.3), 
#    checkRecoComponents = cms.bool(False),
#    pairCut =cms.string(""),
#    requireNoOverlaps = cms.bool(False),
#    ),
#    ),
#   Finalcut = cms.string(''),
#)


#topElectrons = cms.EDProducer("PATElectronCleaner",
#                              src= cms.InputTag("preselectedElectrons"),
#                              preselection = cms.string(''),
#                              checkOverlaps = cms.PSet(
#    myJets = cms.PSet(
#    src = cms.InputTag("preselectedJets"),
#    Preselection = Cms .string('pt > 20'),
#    algorithm = cms.string('byDeltaR'),
#    deltaR = cms.double(0.3), 
#    checkRecoComponents = cms.bool(False),
#    pairCut =cms.string(""),
#    requireNoOverlaps = cms.bool(False),
#    )
#    ),
#                              finalCut = cms.string(''),
#                              )


bJets = cms.EDFilter("PATJetSelector",
                     src = cms.InputTag("topJets"),
#                     src = cms.InputTag("preselectedJets"),
                     cut = cms.string('pt>0'),
                     filter = cms.bool(False)
                     )

#topJetsPF = cms.EDFilter("PATJetSelector",
#                     src = cms.InputTag("cleanPatJetsPFlow"),
#                     src = cms.InputTag("preselectedJets"),
#                     cut = cms.string('pt>0'),
#                     filter = cms.bool(False)
#                     )


bJetsPF = cms.EDFilter("PATJetSelector",
                     src = cms.InputTag("topJetsPF"),
#                     src = cms.InputTag("preselectedJets"),
                     cut = cms.string('pt>0'),
                     filter = cms.bool(False)
                     )


forwardJetsPF = cms.EDFilter("PATJetSelector",
                     src = cms.InputTag("topJetsPF"),
#                     src = cms.InputTag("preselectedJets"),
                     cut = cms.string('pt>0'),
                     filter = cms.bool(False)
                     )

patPFBJets = cms.EDFilter("PATJetSelector",
                          src = cms.InputTag("patJetsPFlow"),
                          cut = cms.string('pt>0'),
                          filter = cms.bool(False)
                          )

antiBJets = cms.EDFilter("PATJetSelector",
                     src = cms.InputTag("topJets"),
#                     src = cms.InputTag("preselectedJets"),
                     cut = cms.string('pt>0'),
                     filter = cms.bool(False)
                     )


forwardJets = cms.EDFilter("PATJetSelector",
                     src = cms.InputTag("topJets"),
#                     src = cms.InputTag("preselectedJets"),
                     cut = cms.string('pt>0'),
                     filter = cms.bool(False)
                     )

allTopLeptons = cms.EDProducer("SingleTopLeptonsProducer",
                                electronsSource = cms.InputTag("preselectedElectrons"),
                                muonsSource = cms.InputTag("preselectedMuons"),
                                jetsSource = cms.InputTag("preselectedJets"),
                                )

#Top Production : candidate made of a lepton, MET and a b-tagged Jet
recoTops = cms.EDProducer("TopProducer",
                                  electronsSource = cms.InputTag("topElectrons"),
                                  muonsSource = cms.InputTag("topMuons"),
                                  jetsSource = cms.InputTag("bJets"),
                          METsSource = cms.InputTag("preselectedMETs"),
#                                  METsSource = cms.InputTag("patMETsPFlow"),
                                )

boostedTops = cms.EDProducer(
    'NamedCompositeCandidateBooster',
    src = cms.InputTag('recoTops'),
    boostSrc = cms.InputTag('recoTops')
    )

boostedForwardJets = cms.EDProducer(
    'PATJetBooster',
    src = cms.InputTag('forwardJets'),
    boostSrc = cms.InputTag('recoTops')
    )

#Part of MC Truth particles production
MCTruthParticles = cms.EDProducer("SingleTopTChannelMCProducer",
                                          genParticlesSource = cms.InputTag("genParticles")
                                          )


#Part for control samples:

#AntiIso cut for QCD: 
topMuonsAntiIso = topMuons.clone(src = cms.InputTag("cleanPatMuons"))
topElectronsAntiIso = topElectrons.clone(src = cms.InputTag("cleanPatElectrons"))


preselectedMuonsAntiIso = topMuons.clone()
preselectedElectronsAntiIso = topElectrons.clone()

topJetsAntiIso = topJets.clone()

bJetsAntiIso = bJets.clone(src=cms.InputTag('topJetsAntiIso'))
antiBJetsAntiIso = antiBJets.clone(src=cms.InputTag('topJetsAntiIso'))
forwardJetsAntiIso = forwardJets.clone(src=cms.InputTag('topJetsAntiIso'))
