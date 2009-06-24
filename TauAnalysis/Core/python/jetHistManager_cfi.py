import FWCore.ParameterSet.Config as cms

jetHistManager = cms.PSet(
  pluginName = cms.string('jetHistManager'),
  pluginType = cms.string('JetHistManager'),
      
  jetSource = cms.InputTag('selectedLayer1JetsAntiOverlapWithLeptonsVetoCumulative'),

  dqmDirectory_store = cms.string('JetQuantities'),

  #requireGenJetMatch = cms.bool(True),
  requireGenJetMatch = cms.bool(False),

  centralJetsToBeVetoed = cms.PSet(
    etMin = cms.vdouble(10., 15., 20., 30.),
    etaMax = cms.vdouble(2.1, 2.5, 3.0),
    alphaMin = cms.vdouble(0., 0.1, 0.2, 0.3, 0.4, 0.5)
  ),
  
  btaggingAlgos = cms.vstring('simpleSecondaryVertex','combinedSecondaryVertex','trackCountingHighEffBJetTags'),
  discriminators = cms.vdouble(2.,0.4,2.5)
)
