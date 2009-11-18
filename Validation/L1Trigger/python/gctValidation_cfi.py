import FWCore.ParameterSet.Config as cms

gctValidation = cms.EDAnalyzer('GctValidation',
    verbose        = cms.untracked.bool(False),
    outputFile     = cms.untracked.string('gctValidation.root'),

    rctEmTag       = cms.InputTag('simRctDigis::HLT'),
    gctIsoEmTag    = cms.InputTag('simGctDigis:isoEm:HLT'),
    gctNonIsoEmTag = cms.InputTag('simGctDigis:nonIsoEm:HLT'),

    rctRegionTag   = cms.InputTag('simRctDigis::HLT'),
    gctEtSumTag    = cms.InputTag('simGctDigis::HLT'),

    gctCenJetTag   = cms.InputTag('simGctDigis:cenJets:HLT'),
    gctForJetTag   = cms.InputTag('simGctDigis:forJets:HLT'),
    gctTauJetTag   = cms.InputTag('simGctDigis:tauJets:HLT'),
                               
    disableROOToutput = cms.untracked.bool(False)
)
