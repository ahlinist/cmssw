import FWCore.ParameterSet.Config as cms
import RecoMET.METProducers.METSigParams_cfi as met_config


nSVfitEventAnalyzer = cms.EDAnalyzer(
    "NSVfitEventAnalyzer",
    met   = cms.InputTag("patMETs"),
    muons = cms.InputTag("cleanPatMuons"),
    elecs = cms.InputTag("cleanPatElectrons"),
    metSignificance = cms.PSet(
        srcPFJets = cms.InputTag('ak5PFJets'),
        srcPFCandidates = cms.InputTag('particleFlow'),
        resolution = met_config.METSignificance_params,
        dRoverlapPFJet = cms.double(0.3),
        dRoverlapPFCandidate = cms.double(0.1),
        )
)
