import FWCore.ParameterSet.Config as cms
  
# module to select Electrons
# See https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string


#################### TRK ##############################

selectAntiTrkIsoAfterMchHLTElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectMchHLTElecForETau"),
     cut = cms.string('trackIso>0')
)

####################### ECAL #############################

selectAntiEcalIsoAfterTrkIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectAntiTrkIsoAfterMchHLTElecForETau"),
     cut = cms.string('ecalIso>3.8')             
)

################### HCAL #################################

selectAntiHcalIsoAfterEcalIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectAntiEcalIsoAfterTrkIsoElecForETau"),
     cut = cms.string('hcalIso>1.5')             
)

#################### ID ##################################

selectAntiIdAfterIsoElecForETau = cms.EDFilter("PATElectronSelector",       
     src = cms.InputTag("selectAntiHcalIsoAfterEcalIsoElecForETau"),   
     cut = cms.string('electronID("robust")>-1')#No Id by default  
)
 
#################### IP ############################

selectAntiIpAfterIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectAntiIdAfterIsoElecForETau"),
     cut = cms.string('abs(gsfTrack.d0)<0.05')  
)


