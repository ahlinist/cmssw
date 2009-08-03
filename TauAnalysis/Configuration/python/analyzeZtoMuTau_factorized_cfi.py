import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analysisSequenceTools import switchHistManagers
from TauAnalysis.Configuration.analysisSequenceTools import replaceHistManagerInputTags

#--------------------------------------------------------------------------------
# import config for event print-out and analysis sequence of Z --> mu + tau-jet events
# defined for the "regular" case without factorization of muon isolation
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

muTauHistManagers_factorizedWithoutMuonIsolation = copy.deepcopy(muTauHistManagers)

muTauHistManagers_factorizedWithMuonIsolation = cms.vstring(
    'genPhaseSpaceEventInfoHistManager',
    'muonHistManager',
    'tauHistManager',
    'vertexHistManager',
    'triggerHistManager'
)

#--------------------------------------------------------------------------------
# define event selection criteria specific to factorization
#--------------------------------------------------------------------------------

# muon candidate selection with "loose" muon isolation criteria applied
evtSelMuonTrkIsoLooseIsolation = copy.deepcopy(evtSelMuonTrkIso)
evtSelMuonTrkIsoLooseIsolation.src_cumulative = cms.InputTag('muonTrkIsoCutLooseIsolation', 'cumulative')
evtSelMuonTrkIsoLooseIsolation.src_individual = cms.InputTag('muonTrkIsoCutLooseIsolation', 'individual')

evtSelMuonEcalIsoLooseIsolation = copy.deepcopy(evtSelMuonEcalIso)
evtSelMuonEcalIsoLooseIsolation.src_cumulative = cms.InputTag('muonEcalIsoCutLooseIsolation', 'cumulative')
evtSelMuonEcalIsoLooseIsolation.src_individual = cms.InputTag('muonEcalIsoCutLooseIsolation', 'individual')

evtSelMuonAntiPionLooseIsolation = copy.deepcopy(evtSelMuonAntiPion)
evtSelMuonAntiPionLooseIsolation.src_cumulative = cms.InputTag('muonAntiPionCutLooseIsolation', 'cumulative')
evtSelMuonAntiPionLooseIsolation.src_individual = cms.InputTag('muonAntiPionCutLooseIsolation', 'individual')

evtSelMuonTrkIPlooseIsolation = copy.deepcopy(evtSelMuonTrkIP)
evtSelMuonTrkIPlooseIsolation.src_cumulative = cms.InputTag('muonTrkIPcutLooseIsolation', 'cumulative')
evtSelMuonTrkIPlooseIsolation.src_individual = cms.InputTag('muonTrkIPcutLooseIsolation', 'individual')

# selection of di-tau candidates composed of combination of tau-jet with "loosely" isolated muon 
evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation = copy.deepcopy(evtSelDiTauCandidateForMuTauAntiOverlapVeto)
evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation.src_cumulative = cms.InputTag('diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation', 'cumulative')
evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation.src_individual = cms.InputTag('diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation', 'individual')

evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation = copy.deepcopy(evtSelDiTauCandidateForMuTauZeroCharge)
evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation.src_cumulative = cms.InputTag('diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation', 'cumulative')
evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation.src_individual = cms.InputTag('diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation', 'individual')

evtSelDiTauCandidateForMuTauAcoplanarity12LooseMuonIsolation = copy.deepcopy(evtSelDiTauCandidateForMuTauAcoplanarity12)
evtSelDiTauCandidateForMuTauAcoplanarity12LooseMuonIsolation.src_cumulative = cms.InputTag('diTauCandidateForMuTauAcoplanarity12CutLooseMuonIsolation', 'cumulative')
evtSelDiTauCandidateForMuTauAcoplanarity12LooseMuonIsolation.src_individual = cms.InputTag('diTauCandidateForMuTauAcoplanarity12CutLooseMuonIsolation', 'individual')

evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation = copy.deepcopy(evtSelDiTauCandidateForMuTauMt1MET)
evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation.src_cumulative = cms.InputTag('diTauCandidateForMuTauMt1METcutLooseMuonIsolation', 'cumulative')
evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation.src_individual = cms.InputTag('diTauCandidateForMuTauMt1METcutLooseMuonIsolation', 'individual')

evtSelDiTauCandidateForMuTauPzetaDiffLooseMuonIsolation = copy.deepcopy(evtSelDiTauCandidateForMuTauPzetaDiff)
evtSelDiTauCandidateForMuTauPzetaDiffLooseMuonIsolation.src_cumulative = cms.InputTag('diTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation', 'cumulative')
evtSelDiTauCandidateForMuTauPzetaDiffLooseMuonIsolation.src_individual = cms.InputTag('diTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation', 'individual')

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

muTauEventDump_factorizedWithoutMuonIsolation = copy.deepcopy(muTauEventDump)
muTauEventDump_factorizedWithoutMuonIsolation.name = cms.string('muTauEventDump_factorizedWithoutMuonIsolation')
muTauEventDump_factorizedWithoutMuonIsolation.output = cms.string("std::cout")
muTauEventDump_factorizedWithoutMuonIsolation.triggerConditions = cms.vstring("")

muTauEventDump_factorizedWithMuonIsolation = copy.deepcopy(muTauEventDump)
muTauEventDump_factorizedWithMuonIsolation.name = cms.string('muTauEventDump_factorizedWithMuonIsolation')
muTauEventDump_factorizedWithMuonIsolation.output = cms.string("std::cout")
muTauEventDump_factorizedWithMuonIsolation.triggerConditions = cms.vstring("")

#--------------------------------------------------------------------------------
# define factorization specific analysis sequences
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

muTauAnalysisSequence_factorizedWithoutMuonIsolation = copy.deepcopy(muTauAnalysisSequence)
switchHistManagers(muTauAnalysisSequence_factorizedWithoutMuonIsolation, muTauHistManagers_factorizedWithoutMuonIsolation)
replaceHistManagerInputTags(muTauAnalysisSequence_factorizedWithoutMuonIsolation,
    [ ["selectedLayer1MuonsTrkIsoCumulative", "selectedLayer1MuonsTrkIsoLooseIsolationCumulative"],
      ["selectedLayer1MuonsEcalIsoCumulative", "selectedLayer1MuonsEcalIsoLooseIsolationCumulative"],
      ["selectedLayer1MuonsPionVetoCumulative", "selectedLayer1MuonsPionVetoLooseIsolationCumulative"],
      ["selectedLayer1MuonsTrkIPcumulative", "selectedLayer1MuonsTrkIPlooseIsolationCumulative"],
      ["selectedMuTauPairsAntiOverlapVetoCumulative", "selectedMuTauPairsAntiOverlapVetoLooseMuonIsolationCumulative"],
      ["selectedMuTauPairsZeroChargeCumulative", "selectedMuTauPairsZeroChargeLooseMuonIsolationCumulative"],
      ["selectedMuTauPairsAcoplanarity12Cumulative", "selectedMuTauPairsAcoplanarity12LooseMuonIsolationCumulative"],
      ["selectedMuTauPairsMt1METcumulative", "selectedMuTauPairsMt1METlooseMuonIsolationCumulative"],
      ["selectedMuTauPairsPzetaDiffCumulative", "selectedMuTauPairsPzetaDiffLooseMuonIsolationCumulative"] ]
) 

muTauAnalysisSequence_factorizedWithMuonIsolation = copy.deepcopy(muTauAnalysisSequence)
switchHistManagers(muTauAnalysisSequence_factorizedWithMuonIsolation, muTauHistManagers_factorizedWithMuonIsolation)
