import FWCore.ParameterSet.Config as cms

electronIsolationEfficiencyCorrection = cms.EDProducer("PATElectronEfficiencyCorrectionProducer",
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/electronIsolationEfficiencyCorrection.root"),
    lutName = cms.string('electronIsolationEfficiencyCorrection'),
    parametrization = cms.PSet(
        src = cms.VInputTag(
            'selectedPatElectronsForElecTauTrkIPcumulative', # collection to be checked first
            'selectedPatElectronsForElecTauPtCumulative',
			'cleanPatElectrons'   # ("fall-back") collection to be checked last
			#'selectedPatElectronsForElecTauPt15Cumulative'   # ("fall-back") collection to be checked last
        ),
        x = cms.string("eta"),
        y = cms.string("pt")
    ),
    noObjectSubstituteValue = cms.double(0.) # weight returned in case all 'src' collections do not contain any entries
)

