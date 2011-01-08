import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce data-formats providing information 
# about Et of jets (CaloTowers) opposite in transverse plane to tau-jet
#--------------------------------------------------------------------------------

tauRecoilEnergyFromJets = cms.EDProducer("PATTauRecoilEnergyFromJetsProducer",
    srcLeptons = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'),
    srcEnergyObjects = cms.InputTag('patJets'),
    etaMin = cms.double(-2.5),
    etaMax = cms.double(+2.5),
    etMin = cms.double(0.),
    dPhiMin = cms.double(2.64),
    verbosity = cms.untracked.int32(0)
)


tauRecoilEnergyFromCaloTowers = cms.EDProducer("PATTauRecoilEnergyFromCaloTowersProducer",
    srcLeptons = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'),
    srcEnergyObjects = cms.InputTag('towerMaker'),
    etaMin = cms.double(0.),
    etaMax = cms.double(+5.),
    etMin = cms.double(0.5),
    dPhiMin = cms.double(2.64),
    verbosity = cms.untracked.int32(0)
)


tauRecoilEnergyFromCaloTowersPt5 = cms.EDFilter("PATTauRecoilEnergyFromCaloTowersSelector",
                                                src = cms.InputTag("tauRecoilEnergyFromCaloTowers"),
                                                cut = cms.string('etSum < 1e5'),
                                                filter = cms.bool(False)
                                                )  



# -------- Loose isolation selection -----------------# 
tauRecoilEnergyFromJetsLooseIsolation = cms.EDProducer("PATTauRecoilEnergyFromJetsProducer",
    srcLeptons = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoLooseIsolationCumulative'),
    srcEnergyObjects = cms.InputTag('patJets'),
    etaMin = cms.double(-2.5),
    etaMax = cms.double(+2.5),
    etMin = cms.double(0.),
    dPhiMin = cms.double(2.64),
    verbosity = cms.untracked.int32(0)
)

tauRecoilEnergyFromCaloTowersLooseIsolation = cms.EDProducer("PATTauRecoilEnergyFromCaloTowersProducer",
    srcLeptons = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoLooseIsolationCumulative'),
    srcEnergyObjects = cms.InputTag('towerMaker'),
    etaMin = cms.double(0.),
    etaMax = cms.double(+5.),
    etMin = cms.double(0.5),
    dPhiMin = cms.double(2.64),
    verbosity = cms.untracked.int32(0)
)

tauRecoilEnergyFromCaloTowersPt5LooseIsolation = cms.EDFilter("PATTauRecoilEnergyFromCaloTowersSelector",
    src = cms.InputTag("tauRecoilEnergyFromCaloTowersLooseIsolation"),
    cut = cms.string('etSum < 1e5'),
    filter = cms.bool(False)
)  




produceTauRecoilEnergy = cms.Sequence( tauRecoilEnergyFromJets 
                                       * tauRecoilEnergyFromCaloTowers
                                       * tauRecoilEnergyFromCaloTowersPt5                                        
                                       * tauRecoilEnergyFromCaloTowersLooseIsolation
                                       * tauRecoilEnergyFromCaloTowersPt5LooseIsolation 
                                       * tauRecoilEnergyFromJetsLooseIsolation)
