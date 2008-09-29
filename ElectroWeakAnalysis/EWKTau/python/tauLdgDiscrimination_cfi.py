import FWCore.ParameterSet.Config as cms


ldgTrkDiscrForETauKinEff = cms.EDProducer("DiscriminationByLdgTrackProd",
     PfTauTag = cms.InputTag("pfRecoTauProducerForETauEff"), 
     UseTracksInstead = cms.bool(True),
     UseChargeHadInstead = cms.bool(False),
     TauEtCut = cms.double(15.),
     TauEtaCut = cms.double(2.5),                                   
     LeadTrkPtCut = cms.double(5.), 	 	
     ChooseSelectionType = cms.string("passKin")
)

ldgTrkDiscrForETauLdgTrkEff = cms.EDProducer("DiscriminationByLdgTrackProd",
     PfTauTag = cms.InputTag("allLayer0TausForETauKinEff"), 
     UseTracksInstead = cms.bool(True),
     UseChargeHadInstead = cms.bool(False),
     TauEtCut = cms.double(15.),
     TauEtaCut = cms.double(2.5),                                   
     LeadTrkPtCut = cms.double(5.), 	 	
     ChooseSelectionType = cms.string("foundLdgTrk")
)

ldgTrkDiscrForETauLdgTrkCutEff = cms.EDProducer("DiscriminationByLdgTrackProd",
     PfTauTag = cms.InputTag("allLayer0TausForETauLdgTrkEff"), 
     UseTracksInstead = cms.bool(True),
     UseChargeHadInstead = cms.bool(False),
     TauEtCut = cms.double(15.),
     TauEtaCut = cms.double(2.5),                                   
     LeadTrkPtCut = cms.double(5.), 	 	
     ChooseSelectionType = cms.string("passLdgTrkPt")
)


ldgTrkDiscrForETauKin = cms.EDProducer("DiscriminationByLdgTrackProd",
     PfTauTag = cms.InputTag("pfRecoTauProducerForETau"), 
     UseTracksInstead = cms.bool(True),
     UseChargeHadInstead = cms.bool(False),
     TauEtCut = cms.double(15.),
     TauEtaCut = cms.double(2.5),                                   
     LeadTrkPtCut = cms.double(5.),	 	
     ChooseSelectionType = cms.string("passKin")
)

ldgTrkDiscrForETauLdgTrk = cms.EDProducer("DiscriminationByLdgTrackProd",
     PfTauTag = cms.InputTag("allLayer0TausForETauKin"), 
     UseTracksInstead = cms.bool(True),
     UseChargeHadInstead = cms.bool(False),
     TauEtCut = cms.double(15.),
     TauEtaCut = cms.double(2.5),                                   
     LeadTrkPtCut = cms.double(5.), 	 	
     ChooseSelectionType = cms.string("foundLdgTrk")
)

ldgTrkDiscrForETauLdgTrkCut = cms.EDProducer("DiscriminationByLdgTrackProd",
     PfTauTag = cms.InputTag("allLayer0TausForETauLdgTrk"), 
     UseTracksInstead = cms.bool(True),
     UseChargeHadInstead = cms.bool(False),
     TauEtCut = cms.double(15.),
     TauEtaCut = cms.double(2.5),                                   
     LeadTrkPtCut = cms.double(5.), 	 	
     ChooseSelectionType = cms.string("passLdgTrkPt")
)






