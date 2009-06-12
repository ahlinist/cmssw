import FWCore.ParameterSet.Config as cms

genPhaseSpaceEventInfoHistManager = cms.PSet(
  pluginName = cms.string('genPhaseSpaceEventInfoHistManager'),
  pluginType = cms.string('GenPhaseSpaceEventInfoHistManager'),
      
  genPhaseSpaceEventInfoSource = cms.InputTag('genPhaseSpaceEventInfo'),

  genJetSource = cms.InputTag('iterativeCone5GenJets'),
  genJetPtMin = cms.double(20.),
  #genJetEtaMin = cms.double(-2.5),
  #genJetEtaMax = cms.double(+2.5),
  
  genParticlesFromZsSource = cms.InputTag('genParticlesFromZs'),

  dqmDirectory_store = cms.string('GenPhaseSpaceEventInfoQuantities')
)
