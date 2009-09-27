import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce collections of pat::Jets for data-driven background estimation methods
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.patJetSelection_cff import *

jetsEta21ForBgEst = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("cleanLayer1Jets"),        
    cut = cms.string('abs(eta) < 2.1'),
    filter = cms.bool(False)
)

jetsEt20ForBgEst = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("jetsEta21ForBgEst"),                     
    cut = cms.string('et > 20.'),
    filter = cms.bool(False)
)

#--------------------------------------------------------------------------------
# collections of pat::Jets speficic to Z --> muon + tau-jet channel
#--------------------------------------------------------------------------------

jetsAntiOverlapWithLeptonsVetoForMuTauBgEstZmumuEnriched = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag("jetsEt20ForBgEst"),                                                                  
    srcNotToBeFiltered = cms.VInputTag(
        "selectedLayer1ElectronsTrkIPcumulative",
        "muonsTrkTightIsolationForBgEst",
        "selectedLayer1TausChargeCumulative"
    ),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

# select jets with quantity alpha > 0.1
# (defined as ratio of sum of charged particle transverse momenta 
#  to sum of charged plus neutral particle transverse momenta)
jetsAlpha0point1ForMuTauBgEstZmumuEnriched = cms.EDFilter("PATJetAlphaSelector",
    src = cms.InputTag("jetsAntiOverlapWithLeptonsVetoForMuTauBgEstZmumuEnriched"),
    alphaMin = cms.double(0.1),
    filter = cms.bool(False)
)

selectJetsForMuTauBgEstZmumuEnriched = cms.Sequence( jetsAntiOverlapWithLeptonsVetoForMuTauBgEstZmumuEnriched
                                                    * jetsAlpha0point1ForMuTauBgEstZmumuEnriched )

jetsAntiOverlapWithLeptonsVetoForMuTauBgEstWplusJetsEnriched = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag("jetsEt20ForBgEst"),                                                                  
    srcNotToBeFiltered = cms.VInputTag(
        "selectedLayer1ElectronsTrkIPcumulative",
        "muonsTrkTightIsolationForBgEst",
        "tausAntiOverlapWithMuonsVetoLooseIsolationForBgEst"
    ),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

jetsAlpha0point1ForMuTauBgEstWplusJetsEnriched = cms.EDFilter("PATJetAlphaSelector",
    src = cms.InputTag("jetsAntiOverlapWithLeptonsVetoForMuTauBgEstWplusJetsEnriched"),
    alphaMin = cms.double(0.1),
    filter = cms.bool(False)
)

selectJetsForMuTauBgEstWplusJetsEnriched = cms.Sequence( jetsAntiOverlapWithLeptonsVetoForMuTauBgEstWplusJetsEnriched
                                                        * jetsAlpha0point1ForMuTauBgEstWplusJetsEnriched )

jetsAntiOverlapWithLeptonsVetoForMuTauBgEstTTplusJetsEnriched = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag("jetsEt20ForBgEst"),                                                                  
    srcNotToBeFiltered = cms.VInputTag(
        "selectedLayer1ElectronsTrkIPcumulative",
        "muonsTrkTightIsolationForBgEst",
        "selectedLayer1TausForMuTauMuonVetoCumulative"
    ),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

jetsEt40ForMuTauBgEstTTplusJetsEnriched = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("jetsAntiOverlapWithLeptonsVetoForMuTauBgEstTTplusJetsEnriched"),                     
    cut = cms.string('et > 40.'),
    filter = cms.bool(False)
)

jetsEt60ForMuTauBgEstTTplusJetsEnriched = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("jetsAntiOverlapWithLeptonsVetoForMuTauBgEstTTplusJetsEnriched"),                     
    cut = cms.string('et > 60.'),
    filter = cms.bool(False)
)

selectJetsForMuTauBgEstTTplusJetsEnriched = cms.Sequence( jetsAntiOverlapWithLeptonsVetoForMuTauBgEstTTplusJetsEnriched
                                                         * jetsEt40ForMuTauBgEstTTplusJetsEnriched
                                                         * jetsEt60ForMuTauBgEstTTplusJetsEnriched )

#--------------------------------------------------------------------------------
# collections of pat::Jets speficic to Z --> electron + tau-jet channel
#--------------------------------------------------------------------------------

jetsAntiOverlapWithLeptonsVetoForElecTauBgEstZeeEnriched = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag("jetsEt20ForBgEst"),                                                                  
    srcNotToBeFiltered = cms.VInputTag(
        "electronsTrkTightIsolationForBgEst",
        "selectedLayer1MuonsTrkIPcumulative",
        "selectedLayer1TausChargeCumulative"
    ),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

# select jets with quantity alpha > 0.1
# (defined as ratio of sum of charged particle transverse momenta 
#  to sum of charged plus neutral particle transverse momenta)
jetsAlpha0point1ForElecTauBgEstZeeEnriched = cms.EDFilter("PATJetAlphaSelector",
    src = cms.InputTag("jetsAntiOverlapWithLeptonsVetoForElecTauBgEstZeeEnriched"),
    alphaMin = cms.double(0.1),
    filter = cms.bool(False)
)

selectJetsForElecTauBgEstZeeEnriched = cms.Sequence( jetsAntiOverlapWithLeptonsVetoForElecTauBgEstZeeEnriched
                                                    * jetsAlpha0point1ForElecTauBgEstZeeEnriched )

jetsAntiOverlapWithLeptonsVetoForElecTauBgEstWplusJetsEnriched = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag("jetsEt20ForBgEst"),                                                                  
    srcNotToBeFiltered = cms.VInputTag(
        "electronsTrkTightIsolationForBgEst",
        "selectedLayer1MuonsTrkIPcumulative",
        "tausAntiOverlapWithElectronsVetoLooseIsolationForBgEst"
    ),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

jetsAlpha0point1ForElecTauBgEstWplusJetsEnriched = cms.EDFilter("PATJetAlphaSelector",
    src = cms.InputTag("jetsAntiOverlapWithLeptonsVetoForElecTauBgEstWplusJetsEnriched"),
    alphaMin = cms.double(0.1),
    filter = cms.bool(False)
)

selectJetsForElecTauBgEstWplusJetsEnriched = cms.Sequence( jetsAntiOverlapWithLeptonsVetoForElecTauBgEstWplusJetsEnriched
                                                        * jetsAlpha0point1ForElecTauBgEstWplusJetsEnriched )

jetsAntiOverlapWithLeptonsVetoForElecTauBgEstTTplusJetsEnriched = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag("jetsEt20ForBgEst"),                                                                  
    srcNotToBeFiltered = cms.VInputTag(
        "electronsTrkTightIsolationForBgEst",
        "selectedLayer1MuonsTrkIPcumulative",
        "selectedLayer1TausForElecTauElectronVetoCumulative"
    ),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

jetsEt40ForElecTauBgEstTTplusJetsEnriched = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("jetsAntiOverlapWithLeptonsVetoForElecTauBgEstTTplusJetsEnriched"),                     
    cut = cms.string('et > 40.'),
    filter = cms.bool(False)
)

jetsEt60ForElecTauBgEstTTplusJetsEnriched = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("jetsAntiOverlapWithLeptonsVetoForElecTauBgEstTTplusJetsEnriched"),                     
    cut = cms.string('et > 60.'),
    filter = cms.bool(False)
)

selectJetsForElecTauBgEstTTplusJetsEnriched = cms.Sequence( jetsAntiOverlapWithLeptonsVetoForElecTauBgEstTTplusJetsEnriched
                                                           * jetsEt40ForElecTauBgEstTTplusJetsEnriched
                                                           * jetsEt60ForElecTauBgEstTTplusJetsEnriched )

selectJetsForBgEst = cms.Sequence( jetsEta21ForBgEst * jetsEt20ForBgEst
                                  * selectJetsForMuTauBgEstZmumuEnriched
                                  * selectJetsForMuTauBgEstWplusJetsEnriched
                                  * selectJetsForMuTauBgEstTTplusJetsEnriched
                                  * selectJetsForElecTauBgEstZeeEnriched
                                  * selectJetsForElecTauBgEstWplusJetsEnriched
                                  * selectJetsForElecTauBgEstTTplusJetsEnriched )




