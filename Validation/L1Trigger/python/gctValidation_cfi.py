import FWCore.ParameterSet.Config as cms

gctValidation = cms.EDAnalyzer('GctValidation',
    verbose        = cms.untracked.bool(False),
    outputFile     = cms.untracked.string('gctValidation.root'),
    rctEmTag       = cms.InputTag('simRctDigis::HLT'),
    gctIsoEmTag    = cms.InputTag('simGctDigis:isoEm:HLT'),
    gctNonIsoEmTag = cms.InputTag('simGctDigis:nonIsoEm:HLT'),
    disableROOToutput = cms.untracked.bool(False)

)

