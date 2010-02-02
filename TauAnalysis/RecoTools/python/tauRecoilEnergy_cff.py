import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce data-formats providing information 
# about Et of jets (CaloTowers) opposite in transverse plane to tau-jet
#--------------------------------------------------------------------------------

tauRecoilEnergyFromJets = cms.EDProducer("PATTauRecoilEnergyFromJetsProducer",
    srcLeptons = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoCumulative'),
    srcEnergyObjects = cms.InputTag('allLayer1Jets'),
    etaMin = cms.double(-2.5),
    etaMax = cms.double(+2.5),
    etMin = cms.double(0.),
    dPhiMin = cms.double(2.64),
    verbosity = cms.untracked.int32(0)
)

tauRecoilEnergyFromJetsPt1 = cms.EDFilter("PATTauRecoilEnergyFromJetsSelector",
    src = cms.InputTag("tauRecoilEnergyFromJets"),
    cut = cms.string('etSum < 1.'),
    filter = cms.bool(False)
)                                      

tauRecoilEnergyFromCaloTowers = cms.EDProducer("PATTauRecoilEnergyFromCaloTowersProducer",
    srcLeptons = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoCumulative'),
    srcEnergyObjects = cms.InputTag('towerMaker'),
    etaMin = cms.double(0.),
    etaMax = cms.double(+5.),
    etMin = cms.double(0.5),
    dPhiMin = cms.double(2.64),
    verbosity = cms.untracked.int32(0)
)

tauRecoilEnergyFromCentralCaloTowers = cms.EDProducer("PATTauRecoilEnergyFromCaloTowersProducer",
    srcLeptons = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoCumulative'),
    srcEnergyObjects = cms.InputTag('towerMaker'),
    etaMin = cms.double(0.),
    etaMax = cms.double(+3.),
    etMin = cms.double(0.5),
    dPhiMin = cms.double(2.64),
    verbosity = cms.untracked.int32(0)
)

tauRecoilEnergyFromForwardCaloTowers = cms.EDProducer("PATTauRecoilEnergyFromCaloTowersProducer",
    srcLeptons = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoCumulative'),
    srcEnergyObjects = cms.InputTag('towerMaker'),
    etaMin = cms.double(+3.),
    etaMax = cms.double(+5.),
    etMin = cms.double(0.5),
    dPhiMin = cms.double(2.64),
    verbosity = cms.untracked.int32(0)
)

tauRecoilEnergyFromCaloTowersPt5 = cms.EDFilter("PATTauRecoilEnergyFromCaloTowersSelector",
    src = cms.InputTag("tauRecoilEnergyFromCaloTowers"),
    cut = cms.string('etSum < 5.'),
    filter = cms.bool(False)
)  

tauRecoilEnergyFromCaloTowersPt10 = cms.EDFilter("PATTauRecoilEnergyFromCaloTowersSelector",
                                                    src = cms.InputTag("tauRecoilEnergyFromCaloTowers"),
                                                    cut = cms.string('etSum < 10.'),
                                                    filter = cms.bool(False)
                                                )


tauRecoilEnergyFromCentralCaloTowersPt5 = cms.EDFilter("PATTauRecoilEnergyFromCaloTowersSelector",
    src = cms.InputTag("tauRecoilEnergyFromCentralCaloTowers"),
    cut = cms.string('etSum < 5.'),
    filter = cms.bool(False)
)  

tauRecoilEnergyFromForwardCaloTowersPt5 = cms.EDFilter("PATTauRecoilEnergyFromCaloTowersSelector",
    src = cms.InputTag("tauRecoilEnergyFromForwardCaloTowers"),
    cut = cms.string('etSum < 3.'),
    filter = cms.bool(False)
)  

# -------- Loose isolation selection -----------------# 
tauRecoilEnergyFromJetsLooseIsolation = cms.EDProducer("PATTauRecoilEnergyFromJetsProducer",
    srcLeptons = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoLooseIsolationCumulative'),
    srcEnergyObjects = cms.InputTag('allLayer1Jets'),
    etaMin = cms.double(-2.5),
    etaMax = cms.double(+2.5),
    etMin = cms.double(0.),
    dPhiMin = cms.double(2.64),
    verbosity = cms.untracked.int32(0)
)

tauRecoilEnergyFromJetsPt1LooseIsolation = cms.EDFilter("PATTauRecoilEnergyFromJetsSelector",
    src = cms.InputTag("tauRecoilEnergyFromJetsLooseIsolation"),
    cut = cms.string('etSum < 1.'),
    filter = cms.bool(False)
)                                      

tauRecoilEnergyFromCaloTowersLooseIsolation = cms.EDProducer("PATTauRecoilEnergyFromCaloTowersProducer",
    srcLeptons = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoLooseIsolationCumulative'),
    srcEnergyObjects = cms.InputTag('towerMaker'),
    etaMin = cms.double(0.),
    etaMax = cms.double(+5.),
    etMin = cms.double(0.5),
    dPhiMin = cms.double(2.64),
    verbosity = cms.untracked.int32(0)
)

tauRecoilEnergyFromCaloTowersPt5LooseIsolation = cms.EDFilter("PATTauRecoilEnergyFromCaloTowersSelector",
    src = cms.InputTag("tauRecoilEnergyFromCaloTowersLooseIsolation"),
    cut = cms.string('etSum < 5.'),
    filter = cms.bool(False)
)  

tauRecoilEnergyFromCaloTowersPt10LooseIsolation = cms.EDFilter("PATTauRecoilEnergyFromCaloTowersSelector",
                                                                  src = cms.InputTag("tauRecoilEnergyFromCaloTowersLooseIsolation"),
                                                                  cut = cms.string('etSum < 10.'),
                                                                  filter = cms.bool(False)
                                                              )


tauRecoilEnergyFromCentralCaloTowersLooseIsolation = cms.EDProducer("PATTauRecoilEnergyFromCaloTowersProducer",
    srcLeptons = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoLooseIsolationCumulative'),
    srcEnergyObjects = cms.InputTag('towerMaker'),
    etaMin = cms.double(0.),
    etaMax = cms.double(+3.),
    etMin = cms.double(0.5),
    dPhiMin = cms.double(2.64),
    verbosity = cms.untracked.int32(0)
)

tauRecoilEnergyFromCentralCaloTowersPt5LooseIsolation = cms.EDFilter("PATTauRecoilEnergyFromCaloTowersSelector",
    src = cms.InputTag("tauRecoilEnergyFromCentralCaloTowersLooseIsolation"),
    cut = cms.string('etSum < 5.'),
    filter = cms.bool(False)
)  

tauRecoilEnergyFromForwardCaloTowersLooseIsolation = cms.EDProducer("PATTauRecoilEnergyFromCaloTowersProducer",
    srcLeptons = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoLooseIsolationCumulative'),
    srcEnergyObjects = cms.InputTag('towerMaker'),
    etaMin = cms.double(+3.),
    etaMax = cms.double(+5.),
    etMin = cms.double(0.5),
    dPhiMin = cms.double(2.64),
    verbosity = cms.untracked.int32(0)
)

tauRecoilEnergyFromForwardCaloTowersPt5LooseIsolation = cms.EDFilter("PATTauRecoilEnergyFromCaloTowersSelector",
    src = cms.InputTag("tauRecoilEnergyFromForwardCaloTowersLooseIsolation"),
    cut = cms.string('etSum < 3.'),
    filter = cms.bool(False)
)  

produceTauRecoilEnergy = cms.Sequence( tauRecoilEnergyFromJets * tauRecoilEnergyFromJetsPt1
                                       * tauRecoilEnergyFromCaloTowers * tauRecoilEnergyFromCaloTowersPt5 * tauRecoilEnergyFromCaloTowersPt10
                                       * tauRecoilEnergyFromCentralCaloTowers * tauRecoilEnergyFromCentralCaloTowersPt5
                                       * tauRecoilEnergyFromForwardCaloTowers * tauRecoilEnergyFromForwardCaloTowersPt5
                                       * tauRecoilEnergyFromJetsLooseIsolation * tauRecoilEnergyFromJetsPt1LooseIsolation
                                       * tauRecoilEnergyFromCaloTowersLooseIsolation * tauRecoilEnergyFromCaloTowersPt5LooseIsolation * tauRecoilEnergyFromCaloTowersPt10LooseIsolation
                                       * tauRecoilEnergyFromCentralCaloTowersLooseIsolation * tauRecoilEnergyFromCentralCaloTowersPt5LooseIsolation
                                       * tauRecoilEnergyFromForwardCaloTowersLooseIsolation * tauRecoilEnergyFromForwardCaloTowersPt5LooseIsolation )
