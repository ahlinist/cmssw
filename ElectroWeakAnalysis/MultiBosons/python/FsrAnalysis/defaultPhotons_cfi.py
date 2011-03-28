import FWCore.ParameterSet.Config as cms

defaultPhotons = cms.EDFilter("PATPhotonSelector",
    src = cms.InputTag("cleanPatPhotonsTriggerMatch"),
    cut = cms.string("""
        superCluster.energy*sin(superCluster.position.theta) > 10 &&
        hadronicOverEm<0.5
        """
    )
)
