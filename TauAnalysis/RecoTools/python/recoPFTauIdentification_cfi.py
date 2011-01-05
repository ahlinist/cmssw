import FWCore.ParameterSet.Config as cms

# produce discriminator summarizing tau id. criteria applied in EWK-tau analyses
# (used as binning variable in tau id. efficiency measurement)
ewkTauId = cms.EDProducer("PFRecoTauDiscriminationByStringCut",
    PFTauProducer = cms.InputTag('hpsTancTaus'),
    Prediscriminants = cms.PSet(
        BooleanOperator = cms.string("and"),
        leadTrack = cms.PSet(
            Producer = cms.InputTag('hpsTancTausDiscriminationByLeadingTrackFinding'),
            cut = cms.double(0.5)
        ),
        leadTrackPt = cms.PSet(
            Producer = cms.InputTag('hpsTancTausDiscriminationByLeadingTrackPtCut'),
            cut = cms.double(0.5)
        ),
        TaNCloose = cms.PSet(
            Producer = cms.InputTag('hpsTancTausDiscriminationByTancLoose'),
            cut = cms.double(0.5)
        ),
        againstMuon = cms.PSet(
            Producer = cms.InputTag('hpsTancTausDiscriminationAgainstMuon'),
            cut = cms.double(0.5)
        ##),
        ##againstElectron = cms.PSet(
        ##    Producer = cms.InputTag('hpsTancTausDiscriminationAgainstElectron'),
        ##    cut = cms.double(0.5)
        )
    ),
    cut = cms.string(
        '(signalPFChargedHadrCands.size() = 1 | signalPFChargedHadrCands.size() = 3)' \
       + ' & abs(charge) = 1' \
       + ' & (leadPFCand().isNonnull() & leadPFCand().mva_e_pi() < 0.6)' 
    )
)
