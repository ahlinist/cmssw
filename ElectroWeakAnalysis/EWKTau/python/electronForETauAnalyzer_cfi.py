import FWCore.ParameterSet.Config as cms

analyzeElectronForETau = cms.EDAnalyzer("ETauElectronAnalyzer",
   ElecInputTags = cms.VInputTag(
                                   "allLayer1ElecForETau"
	                          ,"selectKinElecForETau"
                                  ,"selectMchHLTElecForETau"
             	                  ,"selectTrkIsoAfterMchHLTElecForETau"
                                  ,"selectEcalIsoAfterTrkIsoElecForETau"
                                  ,"selectHcalIsoAfterEcalIsoElecForETau"
                                  ,"selectIdAfterIsoElecForETau"
                                  ,"selectIpAfterIsoElecForETau"
                                 ),
  RefInputTags = cms.VInputTag(
                                ""
                              ),

  NelecsMax = cms.uint32(1),
  DoMatching = cms.bool(False),
  DoVeto = cms.bool(True),
  DoTuning = cms.bool(True),
  EventWeight = cms.double(1.)
)


