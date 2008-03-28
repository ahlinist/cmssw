import FWCore.ParameterSet.Config as cms

siStripClusters.DigiProducersList = cms.VPSet(cms.PSet(
    DigiLabel = cms.string('\0'),
    DigiProducer = cms.string('stripdigi')
))
siPixelClusters.src = 'pixdigi'

