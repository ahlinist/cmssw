import FWCore.ParameterSet.Config as cms

muonTriggerEfficiencyCorrection = cms.EDProducer("PATMuonEfficiencyCorrectionProducer",
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/muonTriggerEfficiencyCorrection.root"),
    lutName = cms.string('muonTriggerEfficiencyCorrection'),
    parametrization = cms.PSet(
        src = cms.VInputTag(
            'selectedPatMuonsTrkIPcumulative', # collection to be checked first
            'selectedPatMuonsTrkIPlooseIsolationCumulative',
            'selectedPatMuonsPt15Cumulative'   # ("fall-back") collection to be checked last
        ),
        x = cms.string("eta"),
        y = cms.string("pt")
    ),
    noObjectSubstituteValue = cms.double(0.) # weight returned in case all 'src' collections do not contain any entries
)

