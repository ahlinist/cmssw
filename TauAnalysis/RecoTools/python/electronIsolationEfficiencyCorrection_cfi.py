import FWCore.ParameterSet.Config as cms

electronIsolationEfficiencyCorrection = cms.EDProducer("PATElectronEfficiencyCorrectionProducer",
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/electronIsolationEfficiencyCorrection.root"),
    lutName = cms.string('electronIsolationEfficiencyCorrection'),
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

