import FWCore.ParameterSet.Config as cms

tauIdEffBinningZtoMuTau_genMatrix3d = cms.PSet(
    name = cms.string("tauIdEffBinningZtoMuTau_genMatrix3d"),
    config = cms.VPSet(
        cms.PSet(
            extractor = cms.PSet(
                pluginType = cms.string("PATTauValExtractor"),
                src = cms.InputTag('tausForTauIdEffZtoMuTauMuonVetoCumulative'),
                value = cms.string("tauID('ewkTauId')")
            ),
            branchName = cms.string('ewkTauId'),
            binning = cms.PSet(
                boundaries = cms.vdouble(0.5),
                min = cms.double(-0.01),
                max = cms.double(1.01)
            )
        ),
        cms.PSet(
            extractor = cms.PSet(
                pluginType = cms.string("PATMuonValExtractor"),
                src = cms.InputTag('muonsForTauIdEffZtoMuTauTrkIPrelIsolationCumulative'),
                value = cms.string("? pt > 0 ? (userIsolation('pat::TrackIso') + userIsolation('pat::EcalIso'))/pt : -1")
            ),
            branchName = cms.string('relMuonIso'),
            binning = cms.PSet(
                boundaries = cms.vdouble(0.06),
                min = cms.double(-0.01),
                max = cms.double(0.18)
            )
        ),
        cms.PSet(
            extractor = cms.PSet(
                pluginType = cms.string("PATMuTauPairValExtractor"),
                src = cms.InputTag('muTauPairsTauIdEffZtoMuTauRelMuonIsolation'),
                value = cms.string("mt1MET")
            ),
            branchName = cms.string('diTauMt1MET'),
            binning = cms.PSet(
                boundaries = cms.vdouble(40.),
                min = cms.double(-0.01),
                max = cms.double(100.)
            )
        )
    )
)

tauIdEffBinningZtoMuTau_genMatrix4d = cms.PSet(
    name = cms.string("tauIdEffBinningZtoMuTau_genMatrix4d"),
    config = cms.VPSet(
        cms.PSet(
            extractor = cms.PSet(
                pluginType = cms.string("PATTauValExtractor"),
                src = cms.InputTag('tausForTauIdEffZtoMuTauMuonVetoCumulative'),
                value = cms.string("tauID('ewkTauId')")
            ),
            branchName = cms.string('ewkTauId'),
            binning = cms.PSet(
                boundaries = cms.vdouble(0.5),
                min = cms.double(-0.01),
                max = cms.double(1.01)
            )
        ),
        cms.PSet(
            extractor = cms.PSet(
                pluginType = cms.string("PATMuonValExtractor"),
                src = cms.InputTag('muonsForTauIdEffZtoMuTauTrkIPrelIsolationCumulative'),
                value = cms.string("? pt > 0 ? (userIsolation('pat::TrackIso') + userIsolation('pat::EcalIso'))/pt : -1")
            ),
            branchName = cms.string('relMuonIso'),
            binning = cms.PSet(
                boundaries = cms.vdouble(0.06),
                min = cms.double(-0.01),
                max = cms.double(0.18)
            )
        ),
        cms.PSet(
            extractor = cms.PSet(
                pluginType = cms.string("PATMuTauPairValExtractor"),
                src = cms.InputTag('muTauPairsTauIdEffZtoMuTauRelMuonIsolation'),
                value = cms.string("mt1MET")
            ),
            branchName = cms.string('diTauMt1MET'),
            binning = cms.PSet(
                boundaries = cms.vdouble(40.),
                min = cms.double(-0.01),
                max = cms.double(100.)
            )
        ),
        cms.PSet(
            extractor = cms.PSet(
                pluginType = cms.string("PATMuTauPairValExtractor"),
                src = cms.InputTag('muTauPairsTauIdEffZtoMuTauRelMuonIsolation'),
                value = cms.string("? leg2.leadPFChargedHadrCand.isNonnull ? abs(leg1.charge + leg2.leadPFChargedHadrCand.charge) : 0")
            ),
            branchName = cms.string('diTauAbsCharge'),
            binning = cms.PSet(
                boundaries = cms.vdouble(0.5),
                min = cms.double(-0.01),
                max = cms.double(+2.01)
            )
        )
    )
)
