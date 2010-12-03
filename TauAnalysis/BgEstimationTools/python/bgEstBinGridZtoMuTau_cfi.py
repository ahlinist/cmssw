import FWCore.ParameterSet.Config as cms

binning_ewkTauId = cms.PSet(
    extractor = cms.PSet(
        pluginType = cms.string("PATTauValExtractor"),
        src = cms.InputTag(''),
        value = cms.string("tauID('ewkTauId')")
    ),
    branchName = cms.string('ewkTauId'),
    binning = cms.PSet(
        boundaries = cms.vdouble(0.5),
        min = cms.double(-0.01),
        max = cms.double(1.01)
    )
)

binning_relMuonIso = cms.PSet(
    extractor = cms.PSet(
        pluginType = cms.string("PATMuonValExtractor"),
        src = cms.InputTag(''),
        value = cms.string("? pt > 0 ? (userIsolation('pat::TrackIso') + userIsolation('pat::EcalIso'))/pt : -1")
    ),
    branchName = cms.string('relMuonIso'),
    binning = cms.PSet(
        boundaries = cms.vdouble(0.06),
        min = cms.double(-0.01),
        max = cms.double(0.18)
    )
)

binning_diTauDPhi12 = cms.PSet(
    extractor = cms.PSet(
        pluginType = cms.string("PATMuTauPairValExtractor"),
        src = cms.InputTag(''),
        value = cms.string("dPhi12")
    ),
    branchName = cms.string('diTauDPhi12'),
    binning = cms.PSet(
        boundaries = cms.vdouble(2.793),
        min = cms.double(-0.01),
        max = cms.double(+3.15)
    )
)

binning_diTauMt1MET = cms.PSet(
    extractor = cms.PSet(
        pluginType = cms.string("PATMuTauPairValExtractor"),
        src = cms.InputTag(''),
        value = cms.string("mt1MET")
    ),
    branchName = cms.string('diTauMt1MET'),
    binning = cms.PSet(
        boundaries = cms.vdouble(40.),
        min = cms.double(-0.01),
        max = cms.double(100.)
    )
)

binning_diTauAbsCharge = cms.PSet(
    extractor = cms.PSet(
        pluginType = cms.string("PATMuTauPairValExtractor"),
        src = cms.InputTag(''),
        value = cms.string("? leg2.leadPFChargedHadrCand.isNonnull ? abs(leg1.charge + leg2.leadPFChargedHadrCand.charge) : -1")
    ),
    branchName = cms.string('diTauAbsCharge'),
    binning = cms.PSet(
        boundaries = cms.vdouble(-0.5, 0.5),
        min = cms.double(-1.01),
        max = cms.double(+2.01)
    )
)

