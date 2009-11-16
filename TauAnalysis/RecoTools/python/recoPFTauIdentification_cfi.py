import FWCore.ParameterSet.Config as cms

# produce discriminator summarizing tau id. criteria applied in EWK-tau analyses
# (used as binning variable in tau id. efficiency measurement)
ewkTauId = cms.EDProducer("PFRecoTauDiscriminationByStringCut",
    PFTauProducer = cms.InputTag('shrinkingConePFTauProducer'),
    Prediscriminants = cms.PSet(
        BooleanOperator = cms.string("and"),
        leadTrack = cms.PSet(
            Producer = cms.InputTag('shrinkingConePFTauDiscriminationByLeadingTrackFinding'),
            cut = cms.double(0.5)
        ),
        leadTrackPt = cms.PSet(
            Producer = cms.InputTag('shrinkingConePFTauDiscriminationByLeadingTrackPtCut'),
            cut = cms.double(0.5)
        ),
        trackIsolation = cms.PSet(
            Producer = cms.InputTag('shrinkingConePFTauDiscriminationByTrackIsolation'),
            cut = cms.double(0.5)
        ),
        ecalIsolation = cms.PSet(
            Producer = cms.InputTag('shrinkingConePFTauDiscriminationByECALIsolation'),
            cut = cms.double(0.5)
        )
    ),
    cut = cms.string('(signalTracks.size() = 1 | signalTracks.size() = 3) && (abs(charge) > 0.5 & abs(charge) < 1.5)')
)
