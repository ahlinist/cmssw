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
  
    bTaggingDiscriminators = cms.VPSet(
        cms.PSet(
            name = cms.string('simpleSecondaryVertex'),
            threshold = cms.double(2.)
        ),
        cms.PSet(
            name = cms.string('combinedSecondaryVertex'),
            threshold = cms.double(0.4)
        ),
        cms.PSet(
            name = cms.string('trackCountingHighEffBJetTags'),
            threshold = cms.double(2.5)
        ),
        cms.PSet(
            name = cms.string('softElectronBJetTags'),
            threshold = cms.double(0.5)
        ),
        cms.PSet(
            name = cms.string('softMuonBJetTags'),
            threshold = cms.double(0.5)
        )
    )
)
