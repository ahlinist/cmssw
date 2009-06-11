import FWCore.ParameterSet.Config as cms

genMatrixBinningZtoMuTau = cms.PSet(
    binner = cms.string("DataBinner"),
    binning = cms.VPSet(
        cms.PSet(
            extractor = cms.PSet(
                pluginType = cms.string("PATMuonValExtractor"),
                src = cms.InputTag('muonTrkIPcutLooseIsolation', 'cumulative'),
                value = cms.string("trackIso + ecalIso")
            ),
            branchName = cms.string('muonIso'),
            binning = cms.PSet(
                boundaries = cms.vdouble(2.01),
                min = cms.double(-0.01),
                max = cms.double(16.)
            )
        ),
        cms.PSet(
            extractor = cms.PSet(
                pluginType = cms.string("PATMuTauPairValExtractor"),
                src = cms.InputTag('diTauCandidateForMuTauAntiOverlapVeto', 'individual'),
                value = cms.string("abs(charge)")
            ),
            branchName = cms.string('diTauAbsCharge'),
            binning = cms.PSet(
                boundaries = cms.vdouble(0.5),
                min = cms.double(-0.01),
                max = cms.double(+2.01)
            )
        ),
        cms.PSet(
            extractor = cms.PSet(
                pluginType = cms.string("PATMuTauPairValExtractor"),
                src = cms.InputTag('diTauCandidateForMuTauAntiOverlapVeto', 'individual'),
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
