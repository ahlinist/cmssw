import FWCore.ParameterSet.Config as cms
import RecoMET.METProducers.METSigParams_cfi as met_config

nSVfitEventAnalyzer = cms.EDAnalyzer("NSVfitEventAnalyzer",
    met   = cms.InputTag("patMETs"),
    leps1 = cms.InputTag("cleanPatMuons"),
    leps2 = cms.InputTag("cleanPatElectrons"),
    type1 = cms.string("lep"),
    type2 = cms.string("lep"),
    metSignificance = cms.PSet(
        srcPFJets = cms.InputTag('ak5PFJets'),
        srcPFCandidates = cms.InputTag('particleFlow'),
        resolution = met_config.METSignificance_params,
        dRoverlapPFJet = cms.double(0.3),
        dRoverlapPFCandidate = cms.double(0.1),
    )
)
