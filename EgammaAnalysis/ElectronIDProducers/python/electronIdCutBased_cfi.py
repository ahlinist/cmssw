import FWCore.ParameterSet.Config as cms

from EgammaAnalysis.ElectronIDProducers.cutBasedElectronId_cfi import *
from EgammaAnalysis.ElectronIDProducers.ptdrElectronId_cfi import *
eidCutBased = cms.EDFilter("EleIdCutBasedRef",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    endcapClusterShapeAssociation = cms.InputTag("islandBasicClusters","islandEndcapShapeAssoc"),
    doPtdrId = cms.bool(True),
    doCutBased = cms.bool(False),
    filter = cms.bool(False),
    barrelClusterShapeAssociation = cms.InputTag("hybridSuperClusters","hybridShapeAssoc"),
    threshold = cms.double(0.5),
    algo_psets = cms.VPSet(cms.PSet(
        PTDR_ID
    ), cms.PSet(
        CutBased_ID
    ))
)


