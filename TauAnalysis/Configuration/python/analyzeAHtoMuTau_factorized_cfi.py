import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.tools.analysisSequenceTools import replaceAnalyzerInputTags

from TauAnalysis.Configuration.analyzeZtoMuTau_factorized_cfi import *

#--------------------------------------------------------------------------------
# import config for event print-out and analysis sequence of MSSM Higgs A/H --> mu + tau-jet events
# defined for the "regular" case without factorization of muon isolation
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeAHtoMuTau_cfi import *

#--------------------------------------------------------------------------------
# define event selection criteria specific to factorization
#--------------------------------------------------------------------------------

# selection of di-tau candidates composed of combination of tau-jet with "loosely" isolated muon
evtSelDiTauCandidateForAHtoMuTauAntiOverlapVetoLooseMuonIsolation = evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto.clone(
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauAntiOverlapVetoLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauAntiOverlapVetoLooseMuonIsolation', 'individual')
)    
evtSelDiTauCandidateForAHtoMuTauZeroChargeLooseMuonIsolation = evtSelDiTauCandidateForAHtoMuTauZeroCharge.clone(
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauZeroChargeCutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauZeroChargeCutLooseMuonIsolation', 'individual')
)    
evtSelDiTauCandidateForAHtoMuTauMt1METlooseMuonIsolation = evtSelDiTauCandidateForAHtoMuTauMt1MET.clone(
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauMt1METcutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauMt1METcutLooseMuonIsolation', 'individual')
)    
evtSelDiTauCandidateForAHtoMuTauPzetaDiffLooseMuonIsolation = evtSelDiTauCandidateForAHtoMuTauPzetaDiff.clone(
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauPzetaDiffCutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauPzetaDiffCutLooseMuonIsolation', 'individual')
)
evtSelDiTauCandidateForAHtoMuTauNonBackToBackLooseMuonIsolation = evtSelDiTauCandidateForAHtoMuTauMt1MET.clone(
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauNonBackToBackCutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauNonBackToBackCutLooseMuonIsolation', 'individual')
)    
evtSelDiTauCandidateForAHtoMuTauValidCollinearApproxLooseMuonIsolation = evtSelDiTauCandidateForAHtoMuTauValidCollinearApprox.clone(
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauValidCollinearApproxCutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauValidCollinearApproxCutLooseMuonIsolation', 'individual')
)    

# central jet veto/b-jet candidate selection
evtSelNonCentralJetEt20bTagLooseMuonIsolation = cms.PSet(
    pluginName = cms.string('evtSelNonCentralJetEt20bTag'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('centralJetEt20bTagVeto', 'cumulative'),
    src_individual = cms.InputTag('centralJetEt20bTagVeto', 'individual')
)
evtSelCentralJetEt20LooseMuonIsolation = cms.PSet(
    pluginName = cms.string('evtSelCentralJetEt20'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('centralJetEt20Cut', 'cumulative'),
    src_individual = cms.InputTag('centralJetEt20Cut', 'individual')
)
evtSelCentralJetEt20bTagLooseMuonIsolation = cms.PSet(
    pluginName = cms.string('evtSelCentralJetEt20bTag'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('centralJetEt20bTagCut', 'cumulative'),
    src_individual = cms.InputTag('centralJetEt20bTagCut', 'individual')
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

muTauEventDump_factorizedWithoutMuonIsolation = muTauEventDump.clone(
    name = cms.string('muTauEventDump_factorizedWithoutMuonIsolation'),
    output = cms.string("std::cout"),
    triggerConditions = cms.vstring()
)    

muTauEventDump_factorizedWithMuonIsolation = muTauEventDump.clone(
    name = cms.string('muTauEventDump_factorizedWithMuonIsolation'),
    output = cms.string("std::cout"),
    triggerConditions = cms.vstring()
)

#--------------------------------------------------------------------------------
# define factorization specific analysis sequences
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

inputTagReplacements = \
  [ [ "selectedLayer1MuonsTrkIsoCumulative", "selectedLayer1MuonsTrkIsoLooseIsolationCumulative" ],
    [ "selectedLayer1MuonsEcalIsoCumulative", "selectedLayer1MuonsEcalIsoLooseIsolationCumulative" ],
    [ "selectedLayer1MuonsPionVetoCumulative", "selectedLayer1MuonsPionVetoLooseIsolationCumulative" ],
    [ "selectedLayer1MuonsTrkIPcumulative", "selectedLayer1MuonsTrkIPlooseIsolationCumulative" ],
    [ "selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative",
      "selectedMuTauPairsForAHtoMuTauAntiOverlapVetoLooseMuonIsolationCumulative" ],
    [ "selectedMuTauPairsForAHtoMuTauZeroChargeCumulative",
      "selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationCumulative" ],
    [ "selectedMuTauPairsForAHtoMuTauAcoplanarity12Cumulative",
      "selectedMuTauPairsForAHtoMuTauAcoplanarity12LooseMuonIsolationCumulative" ],
    [ "selectedMuTauPairsForAHtoMuTauMt1METcumulative",
      "selectedMuTauPairsForAHtoMuTauMt1METlooseMuonIsolationCumulative" ],
    [ "selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative",
      "selectedMuTauPairsForAHtoMuTauPzetaDiffLooseMuonIsolationCumulative" ],
    [ "selectedMuTauPairsForAHtoMuTauNonBackToBackCumulative",
      "selectedMuTauPairsForAHtoMuTauNonBackToBackLooseMuonIsolationCumulative" ],
    [ "selectedMuTauPairsForAHtoMuTauValidCollinearApproxCumulative",
      "selectedMuTauPairsForAHtoMuTauValidCollinearApproxLooseMuonIsolationCumulative" ],
    [ "muTauPairZmumuHypothesesForAHtoMuTau",
      "muTauPairZmumuHypothesesForAHtoMuTauLooseMuonIsolation" ],
    [ "muTauPairZmumuHypothesesForAHtoMuTau",
      "muTauPairZmumuHypothesesForAHtoMuTauLooseMuonIsolation" ],    
    [ "muTauPairVisMassHypothesesForAHtoMuTauNonBtag",
      "muTauPairVisMassHypothesesForAHtoMuTauNonBtagLooseMuonIsolation" ],
    [ "muTauPairVisMassHypothesesForAHtoMuTauBtag",
      "muTauPairVisMassHypothesesForAHtoMuTauBtagLooseMuonIsolation" ],    
    [ "selectedLayer1JetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative",
      "selectedLayer1JetsForAHtoMuTauAntiOverlapWithLeptonsVetoLooseMuonIsolationCumulative" ],
    [ "selectedLayer1JetsForAHtoMuTauBtagCumulative", "selectedLayer1JetsForAHtoMuTauBtagCumulative" ] ]


muTauAnalysisSequence_woBtag_factorizedWithoutMuonIsolation = copy.deepcopy(muTauAnalysisSequence_woBtag)
replaceAnalyzerInputTags(muTauAnalysisSequence_woBtag_factorizedWithoutMuonIsolation, inputTagReplacements)

muTauAnalysisSequence_woBtag_factorizedWithMuonIsolation = copy.deepcopy(muTauAnalysisSequence_woBtag)

muTauAnalysisSequence_wBtag_factorizedWithoutMuonIsolation = copy.deepcopy(muTauAnalysisSequence_wBtag)
replaceAnalyzerInputTags(muTauAnalysisSequence_wBtag_factorizedWithoutMuonIsolation, inputTagReplacements)

muTauAnalysisSequence_wBtag_factorizedWithMuonIsolation = copy.deepcopy(muTauAnalysisSequence_wBtag)
