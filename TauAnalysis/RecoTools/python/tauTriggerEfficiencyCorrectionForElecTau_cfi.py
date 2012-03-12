import FWCore.ParameterSet.Config as cms

tauTriggerEfficiencyCorrectionForElecTau = cms.EDProducer("PATTauEfficiencyCorrectionProducer",
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/tauTriggerEfficiencyCorrectionForElecTau.root"),
    lutName = cms.string('tauTriggerEfficiencyCorrectionForElecTau'),
    parametrization = cms.PSet(
        src = cms.VInputTag(
            'selectedPatTausForElecTauMuonVetoCumulative', # collection to be checked first
            'selectedPatTausElectronVetoCumulative',
            'selectedPatTausPt20Cumulative'   # ("fall-back") collection to be checked last
        ),
        x = cms.string("pt"),
    ),
    noObjectSubstituteValue = cms.double(0.) # weight returned in case all 'src' collections do not contain any entries
)

