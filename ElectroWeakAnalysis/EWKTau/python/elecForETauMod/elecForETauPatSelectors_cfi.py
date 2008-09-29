import FWCore.ParameterSet.Config as cms
  
# module to select Electrons
# See https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string


###################### KINE ##########################

selectKinElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("allLayer1ElecForETau"),
     cut = cms.string('pt > 10. & abs(eta) < 2.5')
)

##################### MATCH HLT ######################
selectMchHLTElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectKinElecForETau"),
     cut = cms.string('triggerMatches.size()')
)              

#################### TRK ##############################

selectTrkIsoElecForETau = cms.EDFilter("PATElectronSelector",       
     src = cms.InputTag("selectKinElecForETau"),   
     cut = cms.string('trackIso=0')   
)

selectTrkIsoAfterMchHLTElecForETau = cms.EDFilter("PATElectronSelector",       
     src = cms.InputTag("selectMchHLTElecForETau"),   
     cut = cms.string('trackIso=0')   
)

####################### ECAL #############################

selectEcalIsoAfterTrkIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectTrkIsoAfterMchHLTElecForETau"),
     cut = cms.string('ecalIso<1.')             
)

selectEcalIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectKinElecForETau"),
     cut = cms.string('ecalIso<1.')             
)


################### HCAL #################################

selectHcalIsoAfterEcalIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectEcalIsoAfterTrkIsoElecForETau"),
     cut = cms.string('hcalIso<2.5')             
)

selectHcalIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectKinElecForETau"),
     cut = cms.string('hcalIso<2.5')             
)


#################### ID ##################################

selectIdAfterIsoElecForETau = cms.EDFilter("PATElectronSelector",       
     src = cms.InputTag("selectHcalIsoAfterEcalIsoElecForETau"),   
     cut = cms.string('leptonID("robust")>0.')  
)
 
selectIdElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectKinElecForETau"),
     cut = cms.string('leptonID("robust")>0.')
)

#################### IP ############################

selectIpAfterIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectIdAfterIsoElecForETau"),
     cut = cms.string('abs(gsfTrack.d0)<1000000.02')  
)

selectIpElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectKinElecForETau"),      
     cut = cms.string('abs(gsfTrack.d0)<0.02')               
)

