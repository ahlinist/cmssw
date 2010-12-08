import FWCore.ParameterSet.Config as cms

muonIsolationEfficiencyCorrection = cms.EDProducer("PATMuonEfficiencyCorrectionProducer",
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/muonIsolationEfficiencyCorrection.root"),
    lutName = cms.string('muonIsolationEfficiencyCorrection'),
    parametrization = cms.PSet(
        srcLeptons = cms.VInputTag(
            'selectedPatMuonsTrkIPcumulative', # collection to be checked first
            'selectedPatMuonsTrkIPlooseIsolationCumulative',
            'selectedPatMuonsPt15Cumulative'   # ("fall-back") collection to be checked last
        ),
        x = cms.string("eta"),
        y = cms.string("pt")
    ),
    noLeptonSubstituteValue = cms.double(0.) # weight returned in case all 'srcLeptons' collections do not contain any entries
)

