import FWCore.ParameterSet.Config as cms
  
# module to select Electrons
# See https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string


###################### KINE ##########################


selectEtaElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("allLayer1ElecForETau"),
     cut = cms.string('abs(superCluster.eta)<1.4442 || abs(superCluster.eta)>1.56')
)

selectKinElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectEtaElecForETau"),
     cut = cms.string(' pt > 18. & abs(eta)<2.1')
)


##################### MATCH HLT ######################
selectMchHLTElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectKinElecForETau"),
     cut = cms.string('triggerMatches.size()')
)              

#################### TRK ##############################

selectTrkIsoElecForETau = cms.EDFilter("PATElectronSelector",       
     src = cms.InputTag("selectKinElecForETau"),   
     cut = cms.string('trackIso<0.9')   
)

selectTrkIsoAfterMchHLTElecForETau = cms.EDFilter("PATElectronSelector",       
     src = cms.InputTag("selectMchHLTElecForETau"),   
     cut = cms.string('trackIso<0.9')   
)

####################### ECAL #############################

selectEcalIsoAfterTrkIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectTrkIsoAfterMchHLTElecForETau"),
     cut = cms.string('(abs(superCluster.eta)<1.479&ecalIso<1.0) | (abs(superCluster.eta)>1.479&ecalIso<2.5)')             
)

selectEcalIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectKinElecForETau"),
     cut = cms.string('(abs(superCluster.eta)<1.479&ecalIso<1.0) | (abs(superCluster.eta)>1.479&ecalIso<2.5)')             
)

################### HCAL #################################

selectHcalIsoAfterEcalIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectEcalIsoAfterTrkIsoElecForETau"),
     cut = cms.string('hcalIso<1.5')             
)

selectHcalIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectKinElecForETau"),
     cut = cms.string('hcalIso<1.5')             
)

#################### ID ##################################

selectIdAfterIsoElecForETau = cms.EDFilter("PATElectronSelector",       
     src = cms.InputTag("selectHcalIsoAfterEcalIsoElecForETau"),   
     cut = cms.string('(abs(superCluster.eta)<1.479 & electronID("robust")>0 & eSuperClusterOverP<1.05 & eSuperClusterOverP>0.95) | (abs(superCluster.eta)>1.479 & electronID("robust")>0 & eSuperClusterOverP<1.12 & eSuperClusterOverP>0.95)')
#    cut = cms.string('electronID("robust")>0&eSuperClusterOverP>0.95&( (abs(superCluster.eta)<1.479&eSuperClusterOverP<1.05) | (abs(superCluster.eta)>1.479&eSuperClusterOverP<1.12) )')
)
 
selectIdElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectKinElecForETau"),
     cut = cms.string('(abs(superCluster.eta)<1.479 & electronID("robust")>0 & eSuperClusterOverP<1.05 & eSuperClusterOverP>0.95) | (abs(superCluster.eta)>1.479 & electronID("robust")>0 & eSuperClusterOverP<1.12 & eSuperClusterOverP>0.95)')
)

#################### IP ############################

selectIpAfterIsoElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectIdAfterIsoElecForETau"),
     cut = cms.string('abs(gsfTrack.d0)<0.05')  
)

selectIpElecForETau = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectKinElecForETau"),      
     cut = cms.string('abs(gsfTrack.d0)<0.05')               
)

