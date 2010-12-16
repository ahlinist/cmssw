import FWCore.ParameterSet.Config as cms

# produce discriminator summarizing tau id. criteria applied in EWK-tau analyses
# (used as binning variable in tau id. efficiency measurement)
ewkTauId = cms.EDProducer("PFRecoTauDiscriminationByStringCut",
    PFTauProducer = cms.InputTag('hpsTancTaus'),
    Prediscriminants = cms.PSet(
        BooleanOperator = cms.string("and"),
        leadTrack = cms.PSet(
            Producer = cms.InputTag('hpsTancTausDiscriminationByLeadingTrackPtCut'),
            cut = cms.double(0.5)
        ),
        leadTrackPt = cms.PSet(
            Producer = cms.InputTag('hpsTancTausDiscriminationByTancMedium'),
            cut = cms.double(0.5)
        ),
    ),
    cut = cms.string('(signalPFChargedHadrCands.size() = 1 | signalPFChargedHadrCands.size() = 3) && (abs(charge) > 0.5 & abs(charge) < 1.5)')
)
