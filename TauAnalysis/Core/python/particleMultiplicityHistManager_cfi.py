import FWCore.ParameterSet.Config as cms

particleMultiplicityHistManager = cms.PSet(
    pluginName = cms.string('particleMultiplicityHistManager'),
    pluginType = cms.string('ParticleMultiplicityHistManager'),

    histograms = cms.VPSet(
        cms.PSet(
            name = cms.string("NumGlobalMuons"),
            title = cms.string("Number of global Muons"),
            numBinsX = cms.uint32(10),
            xMin = cms.double(-0.5),
            xMax = cms.double(9.5),
            src = cms.InputTag('selectedLayer1MuonsGlobalIndividual')
        ),
        cms.PSet(
            name = cms.string("NumTightIdElectrons"),
            title = cms.string("Number of tight id. Electrons"),
            numBinsX = cms.uint32(10),
            xMin = cms.double(-0.5),
            xMax = cms.double(9.5),
            src = cms.InputTag('selectedLayer1ElectronsTightIdIndividual')
        )
    ),

    dqmDirectory_store = cms.string('ParticleMultiplicities')
)
