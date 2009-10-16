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

selectJetsForBgEst = cms.Sequence( jetsEta21ForBgEst * jetsEt20ForBgEst )
