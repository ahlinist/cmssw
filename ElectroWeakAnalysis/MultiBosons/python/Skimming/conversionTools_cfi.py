import FWCore.ParameterSet.Config as cms

conversionTools = cms.EDProducer('ConversionToolsProducer',
    photonSrc = cms.InputTag('photons'),
    electronSrc = cms.InputTag('gsfElectrons'),
    conversionSrc = cms.InputTag('allConversions'), ## 42x and above
    #conversionSrc = cms.InputTag('trackerOnlyConversions'), ## 41x
    beamSpotSrc = cms.InputTag('offlineBeamSpot')
)
