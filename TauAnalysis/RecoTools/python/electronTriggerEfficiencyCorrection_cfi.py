import FWCore.ParameterSet.Config as cms

electronTriggerEfficiencyCorrection = cms.EDProducer("PATElectronEfficiencyCorrectionProducer",
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/electronTriggerEfficiencyCorrection.root"),
    lutName = cms.string('electronTriggerEfficiencyCorrection'),
    parametrization = cms.PSet(
        srcLeptons = cms.VInputTag(
            'selectedPatElectronsForElecTauTrkIPcumulative', # collection to be checked first
            'selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative',
			'cleanPatElectrons'   # ("fall-back") collection to be checked last
			#'selectedPatElectronsForElecTauPt15Cumulative'   # ("fall-back") collection to be checked last
        ),
        x = cms.string("eta"),
        y = cms.string("pt")
    ),
    noLeptonSubstituteValue = cms.double(0.) # weight returned in case all 'srcLeptons' collections do not contain any entries
)

