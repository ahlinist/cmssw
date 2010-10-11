import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.tools.analysisSequenceTools import replaceAnalyzerInputTags

#--------------------------------------------------------------------------------
# import config for event print-out and analysis sequence of Z --> tau-jet + tau-jet events
# defined for the "regular" case without factorization of tau id. criteria
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoDiTau_cfi import *

#--------------------------------------------------------------------------------
# define event selection criteria specific to factorization
#--------------------------------------------------------------------------------

evtSelSecondTauLeadTrkPtLoose = evtSelSecondTauLeadTrkPt.clone(
    src_cumulative = cms.InputTag('secondTauLeadTrkPtCutLoose', 'cumulative'),
    src_individual = cms.InputTag('secondTauLeadTrkPtCutLoose', 'individual')
)
evtSelSecondTauTaNCdiscrLoose = evtSelSecondTauTaNCdiscr.clone(
    src_cumulative = cms.InputTag('secondTauTaNCdiscrCutLoose', 'cumulative'),
    src_individual = cms.InputTag('secondTauTaNCdiscrCutLoose', 'individual')
)
evtSelSecondTauTrkIsoLoose = evtSelSecondTauTrkIso.clone(
    src_cumulative = cms.InputTag('secondTauTrkIsoCutLoose', 'cumulative'),
    src_individual = cms.InputTag('secondTauTrkIsoCutLoose', 'individual')
)
evtSelSecondTauEcalIsoLoose = evtSelSecondTauEcalIso.clone(
    src_cumulative = cms.InputTag('secondTauEcalIsoCutLoose', 'cumulative'),
    src_individual = cms.InputTag('secondTauEcalIsoCutLoose', 'individual')
)
evtSelSecondTauProngLoose = evtSelSecondTauProng.clone(
    src_cumulative = cms.InputTag('secondTauProngCutLoose', 'cumulative'),
    src_individual = cms.InputTag('secondTauProngCutLoose', 'individual')
)
evtSelSecondTauChargeLoose = evtSelSecondTauCharge.clone(
    src_cumulative = cms.InputTag('secondTauChargeCutLoose', 'cumulative'),
    src_individual = cms.InputTag('secondTauChargeCutLoose', 'individual')
)
evtSelSecondTauMuonVetoLoose = evtSelSecondTauMuonVeto.clone(
    src_cumulative = cms.InputTag('secondTauMuonVetoLoose', 'cumulative'),
    src_individual = cms.InputTag('secondTauMuonVetoLoose', 'individual')
)
evtSelSecondTauElectronVetoLoose = evtSelSecondTauElectronVeto.clone(
    src_cumulative = cms.InputTag('secondTauElectronVetoLoose', 'cumulative'),
    src_individual = cms.InputTag('secondTauElectronVetoLoose', 'individual')
)

# di-tau candidate selection
evtSelDiTauCandidateForDiTauAntiOverlapVetoLoose2ndTau = evtSelDiTauCandidateForDiTauAntiOverlapVeto.clone(
    src_cumulative = cms.InputTag('diTauCandidateForDiTauAntiOverlapVetoLoose2ndTau', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForDiTauAntiOverlapVetoLoose2ndTau', 'individual')
)
evtSelDiTauCandidateForDiTauZeroChargeLoose2ndTau = evtSelDiTauCandidateForDiTauZeroCharge.clone(
    src_cumulative = cms.InputTag('diTauCandidateForDiTauZeroChargeCutLoose2ndTau', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForDiTauZeroChargeCutLoose2ndTau', 'individual')
)
evtSelDiTauCandidateForDiTauAcoplanarityLoose2ndTau = evtSelDiTauCandidateForDiTauAcoplanarity.clone(
    src_cumulative = cms.InputTag('diTauCandidateForDiTauAcoplanarityCutLoose2ndTau', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForDiTauAcoplanarityCutLoose2ndTau', 'individual')
)
evtSelDiTauCandidateForDiTauPzetaDiffLoose2ndTau = evtSelDiTauCandidateForDiTauPzetaDiff.clone(
    src_cumulative = cms.InputTag('diTauCandidateForDiTauPzetaDiffCutLoose2ndTau', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForDiTauPzetaDiffCutLoose2ndTau', 'individual')
)

# veto events containing additional central jets with Et > 20 GeV
#evtSelCentralJetVetoLoose2ndTau = cms.PSet(
#    src_cumulative = cms.InputTag('centralJetVetoLoose2ndTau', 'cumulative'),
#    src_individual = cms.InputTag('centralJetVetoLoose2ndTau', 'individual')
#)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

diTauEventDump_factorizedLoose2ndTau = diTauEventDump.clone(
    pluginName = cms.string('diTauEventDump_factorizedLoose2ndTau'),
    svFitAlgorithms = cms.VPSet(
        cms.PSet(
            name = cms.string("psKine_MEt_ptBalance")
        )
    ),
    output = cms.string("std::cout"),
    triggerConditions = cms.vstring()
)    

diTauEventDump_factorizedTight2ndTau = diTauEventDump.clone(
    pluginName = cms.string('diTauEventDump_factorizedTight2ndTau'),
    output = cms.string("std::cout"),
    triggerConditions = cms.vstring()
)

#--------------------------------------------------------------------------------
# define factorization specific analysis sequences
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

inputTagReplacements = \
  [ [ "selectedPatTausForDiTau2ndLeadTrkPtCumulative", "selectedPatTausForDiTau2ndLeadTrkPtLooseCumulative" ],
    [ "selectedPatTausForDiTau2ndTaNCdiscrCumulative", "selectedPatTausForDiTau2ndTaNCdiscrLooseCumulative" ],
    [ "selectedPatTausForDiTau2ndTrkIsoCumulative", "selectedPatTausForDiTau2ndTrkIsoLooseCumulative" ],
    [ "selectedPatTausForDiTau2ndEcalIsoCumulative", "selectedPatTausForDiTau2ndEcalIsoLooseCumulative" ],
    [ "selectedPatTausForDiTau2ndProngCumulative", "selectedPatTausForDiTau2ndProngLooseCumulative" ],
    [ "selectedPatTausForDiTau2ndChargeCumulative", "selectedPatTausForDiTau2ndChargeLooseCumulative" ],
    [ "selectedPatTausForDiTau2ndMuonVetoCumulative", "selectedPatTausForDiTau2ndMuonVetoLooseCumulative" ],
    [ "selectedPatTausForDiTau2ndElectronVetoCumulative", "selectedPatTausForDiTau2ndElectronVetoLooseCumulative" ],    
    [ "selectedDiTauPairs2ndTauLeadTrkPtCumulative", "selectedDiTauPairs2ndTauLeadTrkPtLooseCumulative" ],
    [ "selectedDiTauPairs2ndTauTaNCdiscrCumulative", "selectedDiTauPairs2ndTauTaNCdiscrLooseCumulative" ],
    [ "selectedDiTauPairs2ndTauTrkIsoCumulative", "selectedDiTauPairs2ndTauTrkIsoLooseCumulative" ],
    [ "selectedDiTauPairs2ndTauEcalIsoCumulative", "selectedDiTauPairs2ndTauEcalIsoLooseCumulative" ],
    [ "selectedDiTauPairs2ndTauProngCumulative", "selectedDiTauPairs2ndTauProngLooseCumulative" ],
    [ "selectedDiTauPairs2ndTauChargeCumulative", "selectedDiTauPairs2ndTauChargeLooseCumulative" ],
    [ "selectedDiTauPairs2ndTauMuonVetoCumulative", "selectedDiTauPairs2ndTauMuonVetoLooseCumulative" ],
    [ "selectedDiTauPairs2ndTauElectronVetoCumulative", "selectedDiTauPairs2ndTauElectronVetoLooseCumulative" ],
    [ "selectedDiTauPairsAntiOverlapVetoCumulative", "selectedDiTauPairsAntiOverlapVetoLoose2ndTauCumulative" ],
    [ "selectedDiTauPairsZeroChargeCumulative", "selectedDiTauPairsZeroChargeLoose2ndTauCumulative" ],
    [ "selectedDiTauPairsAcoplanarityCumulative", "selectedDiTauPairsAcoplanarityLoose2ndTauCumulative" ],
    [ "selectedDiTauPairsPzetaDiffCumulative", "selectedDiTauPairsPzetaDiffLoose2ndTauCumulative" ],
    [ "selectedPatJetsForZtoDiTauAntiOverlapWithLeptonsVetoCumulative",
      "selectedPatJetsForZtoDiTauAntiOverlapWithLeptonsVetoLoose2ndTauCumulative" ] ]

diTauAnalysisSequence_factorizedLoose2ndTau = copy.deepcopy(diTauAnalysisSequence)
replaceAnalyzerInputTags(diTauAnalysisSequence_factorizedLoose2ndTau, inputTagReplacements)

diTauAnalysisSequence_factorizedTight2ndTau = copy.deepcopy(diTauAnalysisSequence)
