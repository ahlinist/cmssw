import FWCore.ParameterSet.Config as cms

tauTriggerEfficiencyCorrectionForMuTau = cms.EDProducer("PATTauEfficiencyCorrectionProducer",
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/tauTriggerEfficiencyCorrectionForMuTau.root"),
    lutName = cms.string('tauTriggerEfficiencyCorrectionForMuTau'),
    parametrization = cms.PSet(
        src = cms.VInputTag(
            'selectedPatTausForMuTauElectronVetoCumulative', # collection to be checked first
            'selectedPatTausElectronVetoCumulative',
            'selectedPatTausPt20Cumulative'   # ("fall-back") collection to be checked last
        ),
        x = cms.string("pt"),
    ),
    noObjectSubstituteValue = cms.double(0.) # weight returned in case all 'src' collections do not contain any entries
)

