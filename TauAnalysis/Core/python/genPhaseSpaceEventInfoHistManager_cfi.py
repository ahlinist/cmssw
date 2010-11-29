import FWCore.ParameterSet.Config as cms

genPhaseSpaceEventInfoHistManager = cms.PSet(
  pluginName = cms.string('genPhaseSpaceEventInfoHistManager'),
  pluginType = cms.string('GenPhaseSpaceEventInfoHistManager'),

  generatorInfoSource = cms.InputTag('generator'),
  genPhaseSpaceEventInfoSource = cms.InputTag('genPhaseSpaceEventInfo'),

  genJetSource = cms.InputTag('ak5GenJets'),
  genJetPtMin = cms.double(20.),
  #genJetEtaMin = cms.double(-2.5),
  #genJetEtaMax = cms.double(+2.5),
  
  genParticlesFromZsSource = cms.InputTag('genParticlesFromZs'),
  genParticlesFromHsSource = cms.InputTag('genParticlesFromHs'),

  dqmDirectory_store = cms.string('GenPhaseSpaceEventInfoQuantities'),

  makeBjorkenXratioHistogram = cms.bool(False),
  makeLeptonPtVsPtHatHistograms = cms.bool(False)
)
