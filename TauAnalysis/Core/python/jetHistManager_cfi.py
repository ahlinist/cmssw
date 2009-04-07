import FWCore.ParameterSet.Config as cms

jetHistManager = cms.PSet(
  name = cms.string('jetHistManager'),
  type = cms.string('JetHistManager'),
      
  jetSource = cms.InputTag('selectedLayer1JetsAntiOverlapWithLeptonsVeto'),

  dqmDirectory_store = cms.string('JetQuantities'),

  #requireGenJetMatch = cms.bool(True),
  requireGenJetMatch = cms.bool(False),

  centralJetsToBeVetoed = cms.PSet(
    etMin = cms.vdouble(10., 15., 20., 30.),
    etaMax = cms.vdouble(2.1, 2.5, 3.0),
    alphaMin = cms.vdouble(0., 0.1, 0.2, 0.3, 0.4, 0.5)
  ),
  
  btaggingAlgo = cms.string('trackCountingHighEffBJetTags'),
  discriminator = cms.double(2.)
)
