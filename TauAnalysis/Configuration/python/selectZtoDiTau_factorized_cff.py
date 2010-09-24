import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.selectZtoMuTau_factorized_cff import *

#--------------------------------------------------------------------------------
# import config for selection of Z --> tau-jet + tau-jet events
# defined for the "regular" case without factorization of tau id. criteria
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectZtoDiTau_cff import *

#--------------------------------------------------------------------------------
# define event selection criteria for Z --> tau-jet + tau-jet channel
# specific to factorization
#--------------------------------------------------------------------------------

cfgSecondTauLeadTrkPtCutLoose = cfgSecondTauLeadTrkPtCut.clone(
    pluginName = cms.string('secondTauLeadTrkPtCutLoose'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauLeadTrkPtLooseCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauLeadTrkPtLooseIndividual')
)
cfgSecondTauTaNCdiscrCutLoose = cfgSecondTauTaNCdiscrCut.clone(
    pluginName = cms.string('secondTauTaNCdiscrCutLoose'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauTaNCdiscrLooseCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauTaNCdiscrLooseIndividual')
)
cfgSecondTauTrkIsoCutLoose = cfgSecondTauTrkIsoCut.clone(
    pluginName = cms.string('secondTauTrkIsoCutLoose'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauTrkIsoLooseCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauTrkIsoLooseIndividual')
)
cfgSecondTauEcalIsoCutLoose = cfgSecondTauEcalIsoCut.clone(
    pluginName = cms.string('secondTauEcalIsoCutLoose'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauEcalIsoLooseCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauEcalIsoLooseIndividual')
)
cfgSecondTauProngCutLoose = cfgSecondTauProngCut.clone(
    pluginName = cms.string('secondTauProngCutLoose'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauProngLooseCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauProngLooseIndividual')
)
cfgSecondTauChargeCutLoose = cfgSecondTauChargeCut.clone(
    pluginName = cms.string('secondTauChargeCutLoose'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauChargeLooseCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauChargeLooseIndividual')
)
cfgSecondTauMuonVetoLoose = cfgSecondTauMuonVeto.clone(
    pluginName = cms.string('secondTauMuonVetoLoose'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauMuonVetoLooseCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauMuonVetoLooseIndividual')
)
cfgSecondTauElectronVetoLoose = cfgSecondTauElectronVeto.clone(
    pluginName = cms.string('secondTauElectronVetoLoose'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauElectronVetoLooseCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauElectronVetoLooseIndividual')
)

# di-tau candidate selection
cfgDiTauCandidateForDiTauAntiOverlapVetoLoose2ndTau = cfgDiTauCandidateForDiTauAntiOverlapVeto.clone(
    pluginName = cms.string('diTauCandidateForDiTauAntiOverlapVetoLoose2ndTau'),
    src_cumulative = cms.InputTag('selectedDiTauPairsAntiOverlapVetoLoose2ndTauCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairsAntiOverlapVetoLoose2ndTauIndividual')
)
cfgDiTauCandidateForDiTauZeroChargeCutLoose2ndTau = cfgDiTauCandidateForDiTauZeroChargeCut.clone(
    pluginName = cms.string('diTauCandidateForDiTauZeroChargeCutLoose2ndTau'),
    src_cumulative = cms.InputTag('selectedDiTauPairsZeroChargeLoose2ndTauCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairsZeroChargeLoose2ndTauIndividual')
)
cfgDiTauCandidateForDiTauAcoplanarityCutLoose2ndTau = cfgDiTauCandidateForDiTauAcoplanarityCut.clone(
    pluginName = cms.string('diTauCandidateForDiTauAcoplanarityCutLoose2ndTau'),
    src_cumulative = cms.InputTag('selectedDiTauPairsAcoplanarityLoose2ndTauCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairsAcoplanarityLoose2ndTauIndividual')
)
cfgDiTauCandidateForDiTauPzetaDiffCutLoose2ndTau = cfgDiTauCandidateForDiTauPzetaDiffCut.clone(
    pluginName = cms.string('diTauCandidateForDiTauPzetaDiffCutLoose2ndTau'),
    src_cumulative = cms.InputTag('selectedDiTauPairsPzetaDiffLoose2ndTauCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairsPzetaDiffLoose2ndTauIndividual'),
)

# veto events containing additional central jets with Et > 20 GeV
#cfgCentralJetVetoLoose2ndTau = cfgCentralJetVeto.clone(
#    pluginName = cms.string('centralJetVetoLoose2ndTau'),
#    src = cms.InputTag('selectedPatJetsEt20Cumulative'), # CV: need to carefully check InputTag before using central jet veto !!
#)

zToDiTauEventSelConfiguratorLoose2ndTau = eventSelFlagProdConfigurator(
    [ cfgSecondTauLeadTrkPtCutLoose,
      cfgSecondTauTaNCdiscrCutLoose,
      cfgSecondTauTrkIsoCutLoose,
      cfgSecondTauEcalIsoCutLoose,
      cfgSecondTauProngCutLoose,
      cfgSecondTauChargeCutLoose,
      cfgSecondTauMuonVetoLoose,
      cfgSecondTauElectronVetoLoose,
      cfgDiTauCandidateForDiTauAntiOverlapVetoLoose2ndTau,
      cfgDiTauCandidateForDiTauZeroChargeCutLoose2ndTau,
      cfgDiTauCandidateForDiTauAcoplanarityCutLoose2ndTau,
      cfgDiTauCandidateForDiTauPzetaDiffCutLoose2ndTau ], 
      #cfgCentralJetVetoLoose2ndTau ], 
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectZtoDiTauEventsLoose2ndTau = zToDiTauEventSelConfiguratorLoose2ndTau.configure()
