import FWCore.ParameterSet.Config as cms

from EgammaAnalysis.ElectronIDESSources.likelihoodPdfsDB_cfi import *
from EgammaAnalysis.ElectronIDESSources.likelihoodESetup_cfi import *
eidLikelihood = cms.EDFilter("EleIdLikelihoodRef",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    doLikelihood = cms.bool(True),
    endcapClusterShapeAssociation = cms.InputTag("islandBasicClusters","islandEndcapShapeAssoc"),
    filter = cms.bool(False),
    barrelClusterShapeAssociation = cms.InputTag("hybridSuperClusters","hybridShapeAssoc"),
    threshold = cms.double(0.5)
)


