import FWCore.ParameterSet.Config as cms

ZvZgammaMass = cms.PSet(
    type = cms.untracked.string('TH2F'),
    itemsToPlot = cms.untracked.int32(-1),
    xmin = cms.untracked.double(0),
    xmax = cms.untracked.double(200),
    xnbins = cms.untracked.int32(200),
    ymin = cms.untracked.double(0),
    ymax = cms.untracked.double(200),
    ynbins = cms.untracked.int32(200),
    desc = cms.untracked.string('Z#gamma Mass vs. Daughter 0 Mass [GeV/c^{2}]: M_{Z#gamma} [GeV/c^{2}]: M_{Daughter 0} [GeV/c^{2}]'),
    name = cms.untracked.string('ZvZgammaMass'),
    processFunction = cms.untracked.string('ZvZgammaMass')
    )

zGammaSpecialHistos = cms.VPSet(ZvZgammaMass)
