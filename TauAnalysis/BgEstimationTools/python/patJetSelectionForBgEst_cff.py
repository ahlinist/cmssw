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

jetsAntiOverlapWithLeptonsVetoForMuTauBgEstWplusJetsEnriched = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag("jetsEt20ForBgEst"),                                                                  
    srcNotToBeFiltered = cms.VInputTag("selectedLayer1ElectronsTrkIPcumulative",
                                       "muonsTrkTightIsolationForBgEst",
                                       "tausEcalIsoLooseIsolationForBgEst"),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

# select jets with quantity alpha > 0.1
# (defined as ratio of sum of charged particle transverse momenta 
#  to sum of charged plus neutral particle transverse momenta)
jetsAlpha0point1ForMuTauBgEstWplusJetsEnriched = cms.EDFilter("PATJetAlphaSelector",
    src = cms.InputTag("jetsAntiOverlapWithLeptonsVetoForMuTauBgEstWplusJetsEnriched"),
    alphaMin = cms.double(0.1),
    filter = cms.bool(False)
)

selectJetsForMuTauBgEstWplusJetsEnriched = cms.Sequence( jetsAntiOverlapWithLeptonsVetoForMuTauBgEstWplusJetsEnriched
                                                        * jetsAlpha0point1ForMuTauBgEstWplusJetsEnriched )

jetsAntiOverlapWithLeptonsVetoForMuTauBgEstTTplusJetsEnriched = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag("jetsEt20ForBgEst"),                                                                  
    srcNotToBeFiltered = cms.VInputTag("selectedLayer1ElectronsTrkIPcumulative",
                                       "muonsTrkTightIsolationForBgEst",
                                       "selectedLayer1TausForMuTauMuonVetoCumulative"),
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

selectJetsForBgEst = cms.Sequence( jetsEta21ForBgEst * jetsEt20ForBgEst
                                  * selectJetsForMuTauBgEstWplusJetsEnriched
                                  * selectJetsForMuTauBgEstTTplusJetsEnriched )




