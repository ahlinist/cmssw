import FWCore.ParameterSet.Config as cms


massWithElecDiscrForETau = cms.EDProducer("DiscriminationByMassWithElec",
     PfTauTag = cms.InputTag("allLayer0TausForETauLdgTrkPt"), 
     ElecTag = cms.InputTag("selectIpAfterIsoElecForETau"),
     MinMassCut = cms.double(80.),
     MaxMassCut = cms.double(110.),
     DoSelection = cms.bool(False)
)

massWithElecDiscrForETauEff = cms.EDProducer("DiscriminationByMassWithElec",
     PfTauTag = cms.InputTag("allLayer0TausForETauLdgTrkPtEff"),   
     ElecTag = cms.InputTag("selectIpAfterIsoElecForETau"),
     MinMassCut = cms.double(80.),
     MaxMassCut = cms.double(110.),
     DoSelection = cms.bool(False)
)






