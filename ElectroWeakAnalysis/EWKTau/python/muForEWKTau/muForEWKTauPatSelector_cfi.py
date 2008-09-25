import FWCore.ParameterSet.Config as cms
  
# module to select Muons
# See https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string


###################### KINE ##########################

selectKinMuForEWKTau = cms.EDFilter("PATMuonSelector",
     src = cms.InputTag("allLayer1MuForEWKTau"),
     cut = cms.string('pt > 15. & pt < 40. & abs(eta) < 2.5 & abs(dxy)<0.02')
)

#################### TRK ##############################

selectTrkIsoMuForEWKTau = cms.EDFilter("PATMuonSelector",       
     src = cms.InputTag("selectKinMuForEWKTau"),   
     cut = cms.string('trackIso=0')   
)

####################### ECAL #############################

selectEcalIsoAfterTrkIsoMuForEWKTau = cms.EDFilter("PATMuonSelector",
     src = cms.InputTag("selectTrkIsoMuForEWKTau"),
     cut = cms.string('ecalIso<0.2')             
)

selectEcalIsoMuForEWKTau = cms.EDFilter("PATMuonSelector",
     src = cms.InputTag("selectKinMuForEWKTau"),
     cut = cms.string('ecalIso<1.')             
)


################### HCAL #################################

selectHcalIsoAfterEcalIsoMuForEWKTau = cms.EDFilter("PATMuonSelector",
     src = cms.InputTag("selectEcalIsoAfterTrkIsoMuForEWKTau"),
     cut = cms.string('hcalIso<2.5.')             
)

selectHcalIsoMuForEWKTau = cms.EDFilter("PATMuonSelector",
     src = cms.InputTag("selectKinMuForEWKTau"),
     cut = cms.string('hcalIso<2.5')             
)




