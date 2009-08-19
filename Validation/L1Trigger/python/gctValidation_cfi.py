import FWCore.ParameterSet.Config as cms

gctValidation = cms.EDAnalyzer('GctValidation',
  verbose = cms.bool(False),
  outputFile = cms.string('gctValidation.root'),
  rctTag = cms.string('simRctDigis'),
  gctTag = cms.string('simGctDigis')
)
