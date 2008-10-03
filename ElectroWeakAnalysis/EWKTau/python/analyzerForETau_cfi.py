import FWCore.ParameterSet.Config as cms

analyzeETau = cms.EDAnalyzer("ETauAnalyzer",
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
  TauInputTags = cms.VInputTag(
                                "layer1TausForETauKin"
                               ,"layer1TausForETauLdgTrk"
                               ,"layer1TausForETauLdgTrkPt"
                               ,"layer1TausForETauTrkIsod"
                               ,"layer1TausForETauEclIsod"
                               ,"layer1TausForETauElecRej"
                               ,"select1Or3SigTrackTausForETau"
                                ),

  RefInputTags = cms.VInputTag(
                                "TauRefProducer:ElectronsFromZ"
                               ,"TauRefProducer:ElectronsFromZ"
                              ),

  MetInputTag = cms.InputTag("selectedLayer1METs"), 
  DoMetCuts = cms.bool(False),
  eMetDphiCut = cms.double(2.4),
  eMetMassCut = cms.double(60),
  DoTauIdAfterElecId = cms.bool(True),
  DoMatching = cms.bool(True),
  DoZee = cms.bool(True)
)
